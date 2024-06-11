#include <iostream>
#include "allocator.h"

int main(void) {
	//Allocator allocator_defragmentation_off(4096, true);
	Allocator allocator_defragmentation_on(4096, false);

	/*
	auto ptr1 = allocator_defragmentation_off.allocate(32);
	auto ptr2 = allocator_defragmentation_off.allocate(32);
	auto ptr3 = allocator_defragmentation_off.allocate(32);
	auto ptr4 = allocator_defragmentation_off.allocate(32);

	std::cout << ptr1 << " " << ptr2 << std::endl;
	std::cout << ptr2 << " " << ptr3 << std::endl;
	std::cout << ptr3 << " " << ptr4 << std::endl;
	*/

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

	auto ptr1 = allocator_defragmentation_on.allocate(96);
	auto ptr2 = allocator_defragmentation_on.allocate(32);

	auto ptr4 = allocator_defragmentation_on.allocate(196);

	allocator_defragmentation_on.free(ptr1);
	allocator_defragmentation_on.free(ptr2);//these two will merge together into 1 bigger chunk cause they are adjacent









}