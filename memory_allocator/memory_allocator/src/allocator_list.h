#include "double_linked_list.h"
#include "allocator.h"

//chunk metadata
struct heap_chunk {
    int size; //gives us info about how big the chunk is
    bool in_use; //is the chunk currently in use
};

//allocator list extends doubly linked list and provides additional methods that are exclusively useful to perform on the heap
class AllocatorList : public DoublyLinkedList<heap_chunk*> {
public:
    //overrided print function to include additional info
	virtual void print() override { 
        std::cout << "[";
        Node* temp = this->head;
        while (temp != nullptr) {
            std::cout << "Pointer to data: " << temp->data << " in use: " << (temp->data->in_use ? "TRUE" : "FALSE") << " total_size:" << temp->data->size;
            if (temp->next != nullptr) {
                std::cout << ", " << "\n";
            }
            temp = temp->next;
        }
        std::cout << "]" << "\n" << std::endl;
	}

    void remove_chunk(heap_chunk* chunk) {
        Node* temp = this->head;
        while (temp != nullptr) {
            if (temp->data == chunk) {
                if (temp->prev != nullptr) {
                    temp->prev->next = temp->next;
                }
                else {
                    // Removing the head
                    this->head = temp->next;
                }
                if (temp->next != nullptr) {
                    temp->next->prev = temp->prev;
                }
                else {
                    // Removing the tail
                    this->tail = temp->prev;
                }
                delete temp; // Deleting the node
                this->len--; // Decrement the length
                return;
            }
            temp = temp->next;
        }
        std::cerr << "Error: Chunk not found in the list" << std::endl;
    }

    heap_chunk* find_best_fit(size_t size) {
        auto node = this->get_head();
        heap_chunk* best_fit = nullptr;
        size_t total_size = size + sizeof(heap_chunk);
        int best_dif = total_size;

        // Traverse the list to find the best fit chunk
        while (node != nullptr) {
            heap_chunk* chunk = node->data;
            if (!chunk->in_use && chunk->size >= total_size) {
                if (best_fit == nullptr || chunk->size <= best_fit->size) {
                    if (best_fit != nullptr && chunk->size - best_fit->size < best_dif && chunk->size - best_fit->size > 0) {
                        best_dif = chunk->size - best_fit->size;
                        best_fit = chunk;
                    }
                    else {
                        best_fit = chunk;
                    }
                }
            }
            node = node->next;
        }

        return best_fit;
        
    }

};