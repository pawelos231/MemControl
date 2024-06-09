#include <iostream>
#include "allocator.h"
#include "double_linked_list.h"

int main(void) {
	Allocator al(32);
	DoublyLinkedList<int> dl;
	dl.push(5);
	dl.push(6564);
	dl.pop();
	dl.print();
}