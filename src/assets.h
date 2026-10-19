#ifndef ARM_ASSETS_H
#define ARM_ASSETS_H

// #define CGLTF_IMPLEMENTATION
// #include <cgltf/cgltf.h>

#include "memory.h"

// namespace core {

enum class assets_manager_status : u8 {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR
};

class assets_manager {
public:
    [[nodiscard]] assets_manager_status initialize(void) noexcept;
private:
    static constexpr u32 MAX_MODELS = 64; // lol

    [[nodiscard]] assets_manager_status _read_gltf_file(const char *filepath) noexcept;
    // should i load all at once? streaming?
};

// }

#endif // !ARM_ASSETS_H