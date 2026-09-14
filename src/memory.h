#ifndef ARM_MEMORY_H
#define ARM_MEMORY_H

#include "common.h"

#define MEM_ARENA_DEFAULT_ALIGN alignof(std::max_align_t)
#define MEM_ARENA_HEAD_CAST(type, ptr) static_cast<type>(ptr)

namespace Core {

// typedef struct MemoryArena {
//     u8 *head;
//     size_t used
//     size_t capacity;
// };

class MemoryArena {
public:
    void initialize(void * const head, const size_t capacity);
    void allocate(const size_t size);
    void free(void);
private:
    u8 *head;
    size_t used;
    size_t capacity;
};

}

#endif //ARM_MEMORY_H