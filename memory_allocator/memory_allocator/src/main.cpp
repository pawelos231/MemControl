#include <iostream>
#include "allocator.h"

// Helper function to allocate memory and handle errors

int main(void) {
	try {
		Allocator allocator_defragmentation_off(4096, true); //defragmentation off simply means no merging of chunks
		//Allocator allocator_defragmentation_on(4096, false);

		//BASIC ALLOCATION EXAMPLE
		/*
		auto ptr1 = allocator_defragmentation_off.allocate(32);
		auto ptr2 = allocator_defragmentation_off.allocate(32);
		auto ptr3 = allocator_defragmentation_off.allocate(32);
		auto ptr4 = allocator_defragmentation_off.allocate(32);

		std::cout << ptr1 << " " << ptr2 << std::endl;
		std::cout << ptr2 << " " << ptr3 << std::endl;
		std::cout << ptr3 << " " << ptr4 << std::endl;
		*/

		//ALLOCATING IN PROPER PLACES EXAMPLE MERGING-OFF
		/*
		auto ptr1 = allocator_defragmentation_off.allocate(96);
		auto ptr2 = allocator_defragmentation_off.allocate(32);

		allocator_defragmentation_off.free(ptr1);
		allocator_defragmentation_off.free(ptr2);

		auto ptr3 = allocator_defragmentation_off.allocate(192); //will allocate in the wilderness chunk area
		auto ptr4 = allocator_defragmentation_off.allocate(32); //will allocate in the ptr2 are overriding the metadata
		auto ptr5 = allocator_defragmentation_off.allocate(48); //this will allocate in the ptr1 spot with 72 bytes 24 bytes for metadata and 48 bytes for normal data
		auto ptr6 = allocator_defragmentation_off.allocate(24); //this will allocate right after ptr5 beacuse with 24 bytes for metadata and 24 bytes for normal data
		auto ptr7 = allocator_defragmentation_off.allocate(192);  //this will allocate in the wilderness chunk are
		*/



		//ALLOCATING IN PROPER PLACES EXAMPLE MERGING-ON

		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);

		allocator_defragmentation_on.free(ptr1);
		allocator_defragmentation_on.free(ptr2);

		auto ptr3 = allocator_defragmentation_on.allocate(192);
		auto ptr4 = allocator_defragmentation_on.allocate(32);
		auto ptr5 = allocator_defragmentation_on.allocate(48);
		auto ptr6 = allocator_defragmentation_on.allocate(24);
		auto ptr7 = allocator_defragmentation_on.allocate(192);
		*/

		//MERGING OF TWO ADJECENT CHUNKS EXAMPLE
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);

		auto ptr4 = allocator_defragmentation_on.allocate(196); //we create this chunk to keep it from merging to the wildernes chunk

		allocator_defragmentation_on.free(ptr1);
		allocator_defragmentation_on.free(ptr2);//these two will merge together into 1 bigger chunk cause they are adjacent
		*/


		//MERGING OF FOUR ADJECENT CHUNKS EXAMPLE, DEALLOCATED IN RANDOM ORDER
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);
		auto ptr3 = allocator_defragmentation_on.allocate(64);
		auto ptr4 = allocator_defragmentation_on.allocate(21);

		allocator_defragmentation_on.free(ptr4);
		allocator_defragmentation_on.free(ptr2);
		allocator_defragmentation_on.free(ptr3);
		allocator_defragmentation_on.free(ptr1);
		//they all get merged into the main chunk
		*/


		//MERGING THREE OF FOUR CHUNK INTO ONE BIGGER CHUNK AND ALLOCATING MEMORY
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);
		auto ptr3 = allocator_defragmentation_on.allocate(64);
		auto ptr4 = allocator_defragmentation_on.allocate(21);

		allocator_defragmentation_on.free(ptr1);
		allocator_defragmentation_on.free(ptr2);
		allocator_defragmentation_on.free(ptr3);
		//we get chunk of 264 size
		allocator_defragmentation_on.allocate(192);
		//we are left with 264 - (192+24) = 48 bytes of memory
		//will get allocated into the wilderness chunk taking 74 bytes
		allocator_defragmentation_on.allocate(50);
		//will allocate exactly in the place of the free 48 bytes, taking it
		allocator_defragmentation_on.allocate(24);
		*/

		//MERGE IN DESCENDING ORDER, TO SEE THE WILDERNESS CHUNK GROW
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);
		auto ptr3 = allocator_defragmentation_on.allocate(64);
		auto ptr4 = allocator_defragmentation_on.allocate(21);

		allocator_defragmentation_on.free(ptr4);
		allocator_defragmentation_on.free(ptr3);
		allocator_defragmentation_on.free(ptr2);
		allocator_defragmentation_on.free(ptr1);
		*/

		//ANOTHER TEST CASE OF MERGING THEM IN RANDOM ORDER
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);
		auto ptr2 = allocator_defragmentation_on.allocate(32);
		auto ptr3 = allocator_defragmentation_on.allocate(64);
		auto ptr4 = allocator_defragmentation_on.allocate(21);

		allocator_defragmentation_on.free(ptr1);
		allocator_defragmentation_on.free(ptr4);
		allocator_defragmentation_on.free(ptr3);
		allocator_defragmentation_on.free(ptr2);



		std::cout << ptr4 << std::endl;

		*/

		//TRYING TO DEALLOCATE MEMORY TWICE WILL THROW ERROR
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(96);

		allocator_defragmentation_on.free(ptr1);
		allocator_defragmentation_on.free(ptr1);
		*/

		//TRYING TO ALLOCATE TOO MUCH MEMORY
		/*
		auto ptr1 = allocator_defragmentation_on.allocate(4100);
		*/
	} catch(const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}


}