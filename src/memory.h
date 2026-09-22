#ifndef ARM_MEMORY_H
#define ARM_MEMORY_H

#include "common.h"

static constexpr size_t default_alignment = alignof(std::max_align_t);

namespace core {

// typedef struct MemoryArena {
//     u8 *head;
//     size_t used
//     size_t capacity;
// };

enum class allocator_status : u8 {
    SUCCESS = 0,
    NOT_INITIALIZED,
    OUT_OF_MEMORY,
    INVALID_ARGUMENT,
    INVALID_CAPACITY,
    INVALID_SIZE,
    INVALID_ALIGNMENT
};

class sys_allocator {};

struct allocation_result {
    allocator_status status;
    void *ptr;
};

class linear_allocator {
public:
    [[nodiscard]] allocator_status initialize(void * const head, const size_t capacity);
    [[nodiscard]] allocation_result allocate(const size_t size, const size_t alignment = default_alignment);
    void reset(void);
private:
    u8 *head;
    size_t used;
    size_t capacity;
};

class pool_allocator {};

class double_buffered_allocator {};

}

#endif // !ARM_MEMORY_H