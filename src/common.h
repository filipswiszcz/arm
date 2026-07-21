#ifndef ARM_COMMON_H
#define ARM_COMMON_H

#include <cstdint>

#define ENGINE_LOG(_m, ...) ((void) 0)
#define ENGINE_ASSERT(_e) ((_e) ? 1 : ENGINE_LOG("%s,%d: Assertion '%s' failed: \n", __FILE__, __LINE__, #_e), 0)

typedef float f32;
typedef double f64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

static_assert(sizeof(f32) == 4, "Float must be exactly 4 bytes");
static_assert(sizeof(f64) == 8, "Double must be exactly 8 bytes");

#endif // !ARM_COMMON_H