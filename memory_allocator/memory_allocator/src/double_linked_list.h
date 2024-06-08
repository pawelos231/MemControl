#pragma once

template<typename T>
class DoublyLinkedList {
private:
	//Node class, has prev and next, beacuse the list is double, also has generic T val to make it more versatile
	class Node {
	public:
		T val;
		Node* prev;
		Node* next;

		Node() :next(nullptr), prev(nullptr) {};
		//Without explicit, the compiler can use this constructor for implicit conversions, like: Node<int> n = 5;
		//Here, 5 is implicitly converted to a Node<int> object using the constructor, this can cause problems
		explicit Node(T data) :val(data), next(nullptr), prev(nullptr) {};
	};
	Node* head; //keep track of the head
	Node* tail; //keep track of the tail
	size_t len; //length of the linked list


public:
	DoublyLinkedList();

	//insert element to the linked list at the given index
	insert(int index, T value);

	//remove element to the linked list at the given index
	remove(int index);

	//push at the top of the linked list
	push(T value);

	//push the element to the beggining of the linked list
	unshift(T value);

	//remove the first element from the linked list
	shift();

	//remove the last element from the linked list
	pop();

	//helper function to get the current length of the linked list
	const int length() const;

	//helper function to print the current linkedList
	void print();
};