#pragma once
#include <iostream>
#include "allocator_list.h"
#include "logger.h"

// Structure containing information about the heap.
// - available: The current amount of available memory in bytes.
// - free_list: A doubly linked list of pointers to free chunks of memory.
struct heap_info {
	class AllocatorList free_list;
	size_t available;
};

class Allocator {
public:
// Initializes the allocator with a given pool size and allows control over fragmentation.
// Parameters:
// - pool_size: The total size of the memory pool to be managed by the allocator, in bytes.
// - allow_fragmentation: A boolean value indicating whether fragmentation is allowed.
Allocator(size_t pool_size, bool allow_fragmentation);

// Allocates a block of memory of the specified size.
// Parameters:
// - size: The size of the memory block to allocate, in bytes.
// Returns:
// - A pointer to the allocated memory block, or nullptr if the allocation fails.
void* allocate(size_t size);

// Deallocates a previously allocated block of memory. 
// Parameters:
// - block: A pointer to the memory block to be deallocated.
void free(void* block);

// Prints the current total and free sizes of the heap.
// Useful for debugging and monitoring the memory usage.
void print_size();

// Returns the total size of the memory managed by the allocator, including both used and free memory.
// Returns:
// - The total size of the heap in bytes.
size_t const get_total_size();

// Returns the total free size available in the heap for allocation.
// Returns:
// - The free size of the heap in bytes.
size_t const get_free_size();

private:
	// A boolean flag to control the behavior of fragmentation in the allocator.
	// If set to true, the allocator allows fragmentation without coalescing adjacent free chunks.
	// This is useful for testing the impact of fragmentation compared to solutions where chunk coalescing is performed.
	bool allow_fragmentation;

	// all the info about the heap, available is the current avail size in bytes, free_list is list of pointers to free chunks
	heap_info heap_info;

	// A shared pointer to a singleton Logger instance used for logging allocator events.
	// This allows the allocator to log events such as memory allocations, deallocations, and errors.
	std::shared_ptr<Logger> logger; 

	// Merges adjacent blocks of free chunks to reduce fragmentation and consolidate free space.
	// The underlying doubly linked list structure allows searching for adjacent chunks from both the head and the tail.
	// This operation has a complexity of O(n), where n is the number of free chunks.
	void merge_blocks();
};