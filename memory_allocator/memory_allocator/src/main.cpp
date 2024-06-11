#include <iostream>
#include "allocator.h"

int main(void) {
	//Allocator allocator_defragmentation_off(4096, true);
	Allocator allocator_defragmentation_on(4096, false);



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





	//ALLOCATING IN PROPER PLACES EXAMPLE
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




	//MERGING OF TWO ADJECENT CHUNKS EXAMPLE
	/*
	auto ptr1 = allocator_defragmentation_on.allocate(96);
	auto ptr2 = allocator_defragmentation_on.allocate(32);

	auto ptr4 = allocator_defragmentation_on.allocate(196);

	allocator_defragmentation_on.free(ptr1);
	allocator_defragmentation_on.free(ptr2);//these two will merge together into 1 bigger chunk cause they are adjacent
	*/





	//MERGING OF FOUR ADJECENT CHUNKS EXAMPLE, DEALLOCATED IN DIFFERENT ORDER
	/*
	auto ptr1 = allocator_defragmentation_on.allocate(96);
	auto ptr2 = allocator_defragmentation_on.allocate(32);
	auto ptr3 = allocator_defragmentation_on.allocate(64);
	auto ptr4 = allocator_defragmentation_on.allocate(21);

	allocator_defragmentation_on.free(ptr1);
	allocator_defragmentation_on.free(ptr2);
	allocator_defragmentation_on.free(ptr4);
	allocator_defragmentation_on.free(ptr3);

	//after which we allocate chunk of 192 + 24 bytes, which should allocate in the place of merged chunks leaving 93 bytes of memory shifted by 216 bytes from the previous place
	allocator_defragmentation_on.allocate(192);
	*/


	//TRYING TO DEALLOCATE MEMORY TWICE WILL THROW ERROR
	/*
	auto ptr1 = allocator_defragmentation_on.allocate(96);

	allocator_defragmentation_on.free(ptr1);
	allocator_defragmentation_on.free(ptr1);
	*/



}