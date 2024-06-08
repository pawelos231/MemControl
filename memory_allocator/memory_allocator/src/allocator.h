#pragma once
#include <iostream>

//this will use double linked list, to make it easier to manage the heap
struct heap {
	struct heap_chunk* start; //pointer to the start of the heap
	struct heap_chunk* end; //pointer to the end of the heap
};

struct heap_chunk {
	int size; //gives us info about how big the chunk is
	bool in_use; //is the chunk currently in use
	struct heap_chunk* next; //pointer to the next chunk
};


class Allocator {
public:
	Allocator(size_t pool_size); //pool_size -> how to big to make the 'pool' of space

	void allocate();
	void free();
	void printSize();

	size_t getTotalSize();
	size_t getFreeSize() const;

private:
	void mergeBlocks();
};