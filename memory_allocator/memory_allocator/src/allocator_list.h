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
            std::cout << "Pointer to chunk: " << temp->data << " in use: " << (temp->data->in_use ? "TRUE" : "FALSE") << " total_size:" << temp->data->size;
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
        throw std::runtime_error("Chunk not found in the list");
    }

    //O(n) to find the best fitted chunk for memory that user requested
    heap_chunk* find_best_fit(size_t size) {
        auto node = this->get_head();
        heap_chunk* best_fit = nullptr;
        size_t total_size = size + sizeof(*node);
        int best_dif = int(total_size);

        // Traverse the list to find the best fit chunk
        while (node != nullptr) {
            heap_chunk* chunk = node->data;

            if (!chunk->in_use && chunk->size >= total_size) {
                bool is_better_fit = (best_fit == nullptr) ||
                    (chunk->size < best_fit->size) ||
                    (chunk->size - best_fit->size < best_dif && chunk->size - best_fit->size > 0);

                if (is_better_fit) {
                    best_fit = chunk;
                    best_dif = chunk->size - int(total_size);
                }
            }
            node = node->next;
        }

        if (best_fit == nullptr) {
            throw std::runtime_error("AllocatorList::find_best_fit: Could not find best_fit");
        } 

        return best_fit;
    }

};