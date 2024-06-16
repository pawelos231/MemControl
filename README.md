# MemControl

## Table of Contents

1. [Preliminary Assumptions](#preliminary-assumptions)
2. [Project Description](#project-description)
3. [Used Libraries](#used-libraries)
4. [User Instructions](#user-instructions)
5. [Interesting Facts](#interesting-facts)
6. [Expansion Possibilities](#expansion-possibilities)
7. [Summary and Conclusions](#summary-and-conclusions)

## Preliminary Assumptions

The aim of the project was to create an efficient dynamic memory allocator in C++ that:

- Provides dynamic memory allocation and deallocation.
- Minimizes memory fragmentation.
- Supports portability between different operating systems (Windows and Unix).
- Supplies a logging mechanism for memory allocation and deallocation operations.

## Project Description

The project involves implementing a dynamic memory allocator in C++. The allocator supports dynamic memory allocation and deallocation, managing memory blocks efficiently to minimize fragmentation. A logger is also implemented to monitor allocation and deallocation events as well as potential errors. The project is designed with portability in mind, meaning it works on both Windows and Unix systems.

## Used Libraries

- **C++**: Main programming language used for implementing the program.
- **Standard C++ Library (STL)**: Used for handling data structures and helper functions.
- **System Libraries**: Used for gathering system information, such as `sys/types.h`, `sys/sysinfo.h`, `unistd.h`.

## User Instructions

### Initialization

Use the `Allocator` class constructor to create an allocator instance, specifying the memory pool size and a flag for controlling fragmentation.

**Example**:

```cpp
Allocator allocator(1024 * 1024, true);  // Creates an allocator with 1 MB of memory and enabled fragmentation control.
```

### Memory Allocation

Use the `allocate(size_t size)` method to allocate a memory block of the specified size.

**Example**:

```cpp
void* ptr = allocator.allocate(256);  // Allocates a 256-byte memory block.
```

### Memory Deallocation

Use the `free(void* block)` method to free a previously allocated memory block.

**Example**:

```cpp
allocator.free(ptr);  // Frees the memory block pointed to by `ptr`.
```

### Monitoring

The logger automatically records information about memory allocation and deallocation operations in a log file. The log file is located in the `logs` directory and is named `allocator_logger.txt`.

## Interesting Facts

During the project, we encountered several interesting challenges:

- **Implementation of Custom `mmap` for Windows**: To ensure code portability, we implemented our own version of the `mmap` function for Windows, mimicking the behavior of the Unix function.
- **Memory Fragmentation Management**: One of the main challenges was effectively managing memory fragmentation by merging adjacent free memory blocks, significantly improving allocator performance.
- **Thread-safe Logger**: The logger was designed to be thread-safe, allowing its use in multi-threaded applications without risking data collisions.

## Expansion Possibilities

- **Advanced Memory Management Algorithms**: Implementing more advanced algorithms, such as segregated fits or buddy system, could improve the allocator's efficiency.
- **Logger Expansion**: Adding more comprehensive monitoring and analysis features to the logger.
- **Performance Optimization**: Optimizing the allocator for specific applications to enhance performance.
- **Unit Tests**: Implementing unit tests to ensure reliability and correctness.
- **Dockerization**
- **Simplify build process**

## Summary and Conclusions

The project met the initial assumptions regarding dynamic memory management. The implemented allocator efficiently allocates and frees memory, minimizing fragmentation. The logger provides the necessary information for monitoring memory operations. The project is portable and works on both Windows and Unix systems. Future extensions could include more advanced memory management techniques and additional logger features.
