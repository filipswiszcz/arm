#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    #include <sys/mman.h>
#endif

#include "memory.h"

namespace core {

// SYSTEM ALLOCATOR

allocation_result sys_allocator::allocate(const size_t size) {
    if (!ENGINE_ASSERT(size > 0)) return {allocator_status::INVALID_SIZE, nullptr};

    void *ptr = nullptr;

#if defined(_WIN32)
    ptr = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // raw block of zeroed RAM (unbacked by a file)
    if (ptr == MAP_FAILED) {
        ptr = nullptr;
        ENGINE_LOG("SYS_ALLOC_ERR: Failed to create a new mapping!");
    }
#else
    ENGINE_LOG("SYS_ALLOC_ERR: Platform not supported!");
#endif

    if (!ENGINE_ASSERT(ptr != nullptr)) {
        return {allocator_status::OUT_OF_MEMORY, nullptr};
    }

    return {allocator_status::SUCCESS, ptr};
}

allocator_status sys_allocator::free(void * const ptr, size_t size) {
    if (!ENGINE_ASSERT(ptr != nullptr)) return allocator_status::INVALID_ARGUMENT;

#if defined(_WIN32)
    if (VirtualFree(ptr, 0, MEM_RELEASE) == 0) {
        return allocator_status::UNMAP_FAIL;
    }
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    if (munmap(ptr, size) != 0) {
        return allocator_status::UNMAP_FAIL;
    }
#endif

    return allocator_status::SUCCESS;
}

// LINEAR ALLOCATOR

allocator_status linear_allocator::initialize(void * const head, const size_t capacity) {
    if (!ENGINE_ASSERT(head != nullptr)) return allocator_status::INVALID_ARGUMENT;
    if (!ENGINE_ASSERT(capacity > 0)) return allocator_status::INVALID_CAPACITY;

    this->head = static_cast<u8*>(head);
    this->used = 0;
    this->capacity = capacity;

    return allocator_status::SUCCESS;
}

allocation_result linear_allocator::allocate(const size_t size, const size_t alignment) {
    if (!ENGINE_ASSERT(size > 0)) return {allocator_status::INVALID_SIZE, nullptr};
    if (!ENGINE_ASSERT((alignment & (alignment - 1)) == 0)) return {allocator_status::INVALID_ALIGNMENT, nullptr};

    const uintptr_t caddr = reinterpret_cast<uintptr_t>(this->head + this->used);
    const uintptr_t naddr = (caddr + (alignment - 1)) & ~(alignment - 1);

    const size_t padding = naddr - caddr;

    if (this->used + padding + size > this->capacity) {
        return {allocator_status::OUT_OF_MEMORY, nullptr};
    }

    // size_t available = this->capacity - this->used;
    // if (available < padding || (available -= padding) < size) {
    //     return {allocator_status::OUT_OF_MEMORY, nullptr};
    // }

    this->used += (padding + size);

    return {allocator_status::SUCCESS, reinterpret_cast<void*>(naddr)};
}

void linear_allocator::reset(void) {
    this->used = 0;
}

}