#pragma once

class Allocator {
public:
	Allocator(void* memoryPool, size_t pool_size);

	void allocate();
	void free();
	void printSize();

	size_t getTotalSize();
	size_t getFreeSize() const;

private:
	void mergeBlocks();
};