#include "allocator.h"
#include "logger.h"
#include "mmap_windows.h"


Allocator::Allocator(size_t pool_size) {
    //setup logger
    auto logger = Logger::get_instance();
    logger->set_output_file("allocator_logger.txt");

    //allocate memory from kernel, mmap is basically a system call where you ask a kernel for more memory
    //the only catch is, it is native to unix based systems, which windows is not, that is why there is a mmap_windows.h
    //which provides mmap implementation for windows, probably will have to do something with it later (?)
    void* start = mmap(NULL, pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    //we check if the mmap failed with this, beacuse: #define MAP_FAILED ((void *) -1)
    if (start == (void*)-1) {
        logger->log(Logger::LOG_LEVEL::ERR, "Allocator: mmap error");
        throw std::runtime_error("Allocator: mmap error ");
    }

    std::cout << start << std::endl;
}

void Allocator::allocate()
{

}
