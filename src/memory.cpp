#include "memory.h"

namespace core {

// public

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
    if (!ENGINE_ASSERT(alignment & (alignment - 1) == 0)) return {allocator_status::INVALID_ALIGNMENT, nullptr};

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