#pragma once
#include <iostream>

// Constant for invalid index error message
const char* INVALID_INDEX = "Index out of range";

template <typename T>
class DoublyLinkedList {
private:
    // Node class to represent each element in the list
    class Node {
    public:
        T data;       // Data held by the node
        Node* prev;   // Pointer to the previous node
        Node* next;   // Pointer to the next node

        Node() : prev(nullptr), next(nullptr) {} // Default constructor
        explicit Node(T data) : data(data), prev(nullptr), next(nullptr) {} // Parameterized constructor
    };

    Node* head; // Pointer to the head of the list
    Node* tail; // Pointer to the tail of the list
    int len;    // Length of the list

public:
    // Constructor to initialize an empty list
    DoublyLinkedList() : head(nullptr), tail(nullptr), len(0) {}

    // Function to get the current length of the list
    const int length() const {
        return len;
    }

    // Function to add a new element at the end of the list
    void push(T data) {
        Node* newNode = new Node(data); // Create a new node

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        this->len++; // Increment the length of the list
    }

    // Function to remove the last element of the list
    void pop() {
        if (this->tail == nullptr) {
            throw std::out_of_range(INVALID_INDEX); // Throw an exception if the list is empty
        }

        Node* temp = this->tail; // Temporary pointer to the current tail
        this->tail = this->tail->prev; // Move the tail to the previous node

        if (this->tail != nullptr) {
            this->tail->next = nullptr; // Set the next pointer of the new tail to nullptr
        }
        else {
            this->head = nullptr; // If the list is now empty, set head to nullptr
        }

        delete temp; // Delete the old tail
        this->len--; // Decrement the length of the list
    }

    // Function to remove the first element of the list
    void shift() {
        if (this->head == nullptr) {
            throw std::out_of_range(INVALID_INDEX); // Throw an exception if the list is empty
        }

        Node* temp = this->head; // Temporary pointer to the current head
        this->head = this->head->next; // Move the head to the next node

        if (this->head != nullptr) {
            this->head->prev = nullptr; // Set the prev pointer of the new head to nullptr
        }
        else {
            this->tail = nullptr; // If the list is now empty, set tail to nullptr
        }

        delete temp; // Delete the old head
        this->len--; // Decrement the length of the list
    }

    // Function to add a new element at the beginning of the list
    void unshift(T value) {
        Node* newNode(new Node(value)); // Create a new node

        if (this->head == nullptr) {
            this->head = newNode;
            this->tail = newNode;
            return;
        }
        else {
            newNode->next = this->head;
            this->head->prev = newNode;
            this->head = newNode;
        }

        this->len++; // Increment the length of the list
    }

    // Function to insert an element at a given index
    void insert(int index, T data) {
        if (index < 0 || index > len) {
            throw std::out_of_range(INVALID_INDEX); // Throw an exception if the index is out of range
        }

        if (index == len) {
            push(data); // Use push if inserting at the end
            return;
        }

        Node* newNode = new Node(data); // Create a new node

        if (index == 0) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else {
            Node* temp = head;
            for (int i = 0; i < index - 1; i++) {
                temp = temp->next; // Traverse to the node before the insertion point
            }

            newNode->prev = temp;
            newNode->next = temp->next;
            temp->next->prev = newNode;
            temp->next = newNode;
        }

        len++; // Increment the length of the list
    }

    // Function to remove an element at a given index
    void remove(int index) {
        if (index < 0 || index >= len) {
            throw std::out_of_range(INVALID_INDEX); // Throw an exception if the index is out of range
        }

        Node* temp = head;

        if (index == 0) {
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            else {
                tail = nullptr; // If the list becomes empty, set tail to nullptr
            }
        }
        else if (index == len - 1) {
            temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            for (int i = 0; i < index; i++) {
                temp = temp->next; // Traverse to the node to be removed
            }
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }

        delete temp; // Delete the node
        len--; // Decrement the length of the list
    }

    // Function to print the list
    void print() {
        std::cout << "[";
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data;
            if (temp->next != nullptr) {
                std::cout << ", ";
            }
            temp = temp->next;
        }
        std::cout << "]" << std::endl;
    }
};
