#include "allocator.h"

#if defined(_WIN32) || defined(_WIN64)
#include "mmap_windows.h" // own mmap implementation for windows systems
#else
#include <sys/mman.h> // For mmap on Unix-like systems
#endif


Allocator::Allocator(size_t pool_size, bool allow_fragmentation): allow_fragmentation(allow_fragmentation) {
    //setup logger
    this->logger = Logger::get_instance();
    this->logger->set_output_file("allocator_logger.txt");

    //give memory from kernel, mmap is basically a system call where you ask a kernel for more memory
    //the only catch is, it is native to unix based systems, which windows is not, that is why there is a mmap_windows.h
    //which provides mmap implementation for windows, probably will have to do something with it later (?)
    void* start = mmap(NULL, pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    //we check if the mmap failed with this, beacuse: #define MAP_FAILED ((void *) -1)
    if (start == (void*)-1) {
        this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator: mmap error");
        throw std::runtime_error("Allocator: mmap error ");
    }

    std::cout << "START: " << start << std::endl;

    // first we create the initial region, this is the "wilderness" chunk
    // the heap starts as just one big chunk of allocatable memory, minus the metadata size 
    heap_chunk* init_region = reinterpret_cast<heap_chunk*>(start);
    this->heap_info.free_list.push(init_region);

    init_region->in_use = false;
    init_region->size = int(pool_size) - sizeof(*this->heap_info.free_list.get_head()); //we take into account also all the metadata so prev (pointer), next (pointer), and heap_chunk data (in_use and size)

    // Add the wilderness chunk to the heap linked list
    this->heap_info.available = init_region->size;

    // Log successful initialization
    this->logger->log(Logger::LOG_LEVEL::INFO, "Allocator: Initialized with pool size " + std::to_string(this->heap_info.available) + " bytes");
    return;

}

void* Allocator::allocate(size_t size) {
    // Check if the requested size is greater than the available size
    if (size > this->heap_info.available) {
        this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator::allocate: the size you provided is bigger than the available size");
        return nullptr;
    }
    
    try {
        // Get the head chunk (under node->data)
        auto head = this->heap_info.free_list.get_head();

        // Find the best fit chunk for the requested size
        auto chunk = this->heap_info.free_list.find_best_fit(size);

        // Check if the chunk is not in use and has enough space to accommodate the requested size plus metadata
        if (chunk && !chunk->in_use && chunk->size >= size + sizeof(*head)) {
            // Allocate from this chunk
            if (chunk->size == size + sizeof(*head)) {
                // Exact fit, remove the chunk from the free list
                this->heap_info.free_list.remove_chunk(chunk);
            }
            else if (chunk->size > size + sizeof(*head)) {
                // Split the chunk if it's larger than needed
                heap_chunk* new_chunk = reinterpret_cast<heap_chunk*>(reinterpret_cast<char*>(chunk) + sizeof(*head) + size);
                new_chunk->size = chunk->size - (int(size) + sizeof(*head));
                new_chunk->in_use = false;

                chunk->size = int(size) + sizeof(*head);

                // Remove the original chunk from the free list
                this->heap_info.free_list.remove_chunk(chunk);

                // Add the new chunk to the free list
                if (this->heap_info.free_list.length() == 0) {
                    this->heap_info.free_list.push(new_chunk);
                }
                else {
                    this->heap_info.free_list.unshift(new_chunk);
                }
            }

            // Mark the chunk as in use
            chunk->in_use = true;

            // Update the available size in the heap info
            this->heap_info.available -= (size + sizeof(*head));

            // Print the free list for debugging
            this->heap_info.free_list.print();

            // Return a pointer to the allocated memory, offset by the metadata size
            //std::cout << chunk << " " << sizeof(*head) << " " << reinterpret_cast<void*>(reinterpret_cast<char*>(chunk) + 24) << std::endl;
            return reinterpret_cast<void*>(reinterpret_cast<char*>(chunk) + sizeof(*head));
        }

        // If no suitable chunk was found, throw error and return nullptr
        throw std::runtime_error("No suitable chunk found");
    
    } catch (const std::exception& e) {
        this->logger->log(Logger::LOG_LEVEL::ERR, std::string("Allocator::allocate: Exception occurred: ") + e.what());
    } catch (...) {
        this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator::allocate: An unknown error occurred");
    }

    return nullptr;
    
}


void Allocator::free(void* block) {
        if (block == nullptr) {
            this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator::free: You cannot pass nullptr as the value");
            return;
        };

        auto head = this->heap_info.free_list.get_head();

        // Get the chunk associated with this pointer
        heap_chunk* chunk = reinterpret_cast<heap_chunk*>(reinterpret_cast<char*>(block) - sizeof(*head)); //to get access to the chunk we must substract metadata, beacuse then we will be at the metadata position and would be able to work from there
        if (!chunk->in_use) {
            this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator::free: Double free detected");
            return;
        }


        try {
            // Add the chunk back to the free list at the appropiate position to keep the data in correct order
            int idx = 0;
            auto traverse_node = this->heap_info.free_list.get_head();
            while (chunk > traverse_node->data) {
                traverse_node = traverse_node->next;
                idx++;
            }
            this->heap_info.free_list.insert(idx, chunk);

            // Merge adjacent free chunks to reduce fragmentation
            if (!this->allow_fragmentation) {
                this->merge_blocks();
            }

            chunk->in_use = false;
            this->heap_info.available += chunk->size + sizeof(*head);

            this->heap_info.free_list.print();
        }
        catch (const std::exception& e) {
            this->logger->log(Logger::LOG_LEVEL::ERR, std::string("Allocator::free: Exception occurred: ") + e.what());
        }
        catch (...) {
            this->logger->log(Logger::LOG_LEVEL::ERR, "Allocator::free: An unknown error occurred");
        }   
}

void Allocator::merge_blocks() {
    //get the head and traverse forwards
    auto node = this->heap_info.free_list.get_head();
    while (node != nullptr) {
        auto next = node->next;
        //if next is not a nullptr we can proceed to hopefuly merging
        if (next != nullptr) {
            //declare current and next chunk
            heap_chunk* current_chunk = node->data;
            heap_chunk* next_chunk = next->data;
            void* current_end = reinterpret_cast<void*>(reinterpret_cast<char*>(current_chunk) + current_chunk->size);

            if (current_end == reinterpret_cast<void*>(next_chunk)) {
                // Merge the two chunks
                current_chunk->size += next_chunk->size;
                this->heap_info.free_list.remove_chunk(next_chunk);
            }
            else {
                node = node->next;
            }
        }
        else {
            break;
        }
    }


    // Now merge chunks in the previous direction if possible
    node = this->heap_info.free_list.get_tail();
    while (node != nullptr) {
        auto prev = node->prev;
        if (prev != nullptr) {
            //declare current and prev chunk
            heap_chunk* current_chunk = node->data;
            heap_chunk* prev_chunk = prev->data;
            //get the current ending of the chunk
            void* current_end = reinterpret_cast<void*>(reinterpret_cast<char*>(current_chunk) + current_chunk->size);
            //std::cout << current_end << " " << prev_chunk << std::endl;
            //if the current end is the same as where the prev chunk is located, then merge
            if (current_end == reinterpret_cast<void*>(prev_chunk)) {
                // Merge the two chunks
                current_chunk->size += prev_chunk->size;
                //remove the chunk from the free list, beacuse it has been merged into the current_chunk
                this->heap_info.free_list.remove_chunk(prev_chunk);
            }
            else {
                //iterate
                node = node->prev;
            }
        }
        else {
            break;
        }
    }
};


void Allocator::print_size() {
    size_t totalSize = this->get_total_size();
    size_t freeSize = this->get_free_size();

    this->logger->info("Total size: " + std::to_string(totalSize) + " bytes");
    this->logger->info("Free size: " + std::to_string(freeSize) + " bytes");

    std::cout << "Total size: " << totalSize << " bytes\n";
    std::cout << "Free size: " << freeSize << " bytes\n";
}

size_t const Allocator::get_total_size() {
    size_t totalSize = 0;
    auto current = this->heap_info.free_list.get_head();
    while (current != nullptr) {
        totalSize += current->data->size + sizeof(heap_chunk);
        current = current->next;
    }
    return totalSize;
}

size_t const Allocator::get_free_size() {
    size_t freeSize = 0;
    auto current_node = this->heap_info.free_list.get_head();
    while (current_node != nullptr) {
        if (!current_node->data->in_use) {
            freeSize += current_node->data->size;
        }
        current_node = current_node->next;
    }
    return freeSize;
}