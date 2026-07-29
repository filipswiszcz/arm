#ifndef ARM_COMMON_H
#define ARM_COMMON_H

#include <cstdint>

#include "math.h"

#define ENGINE_LOG(_m, ...) ((void) 0)
#define ENGINE_ASSERT(_e) ((_e) ? 1 : (ENGINE_LOG("%s,%d: Assertion '%s' failed: \n", __FILE__, __LINE__, #_e), 0))

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
typedef Vec2_t v2;
typedef Vec3_t v3;
typedef Vec4_t v4;
// typedef Mat2_t m2;
// typedef Mat3_t m3;
typedef Mat4_t m4;

static_assert(sizeof(f32) == 4, "Float must be exactly 4 bytes");
static_assert(sizeof(f64) == 8, "Double must be exactly 8 bytes");
static_assert(sizeof(v2) == 8, "Vec2 must be exactly 8 bytes");
static_assert(sizeof(v3) == 12, "Vec3 must be exactly 12 bytes");
static_assert(sizeof(v4) == 16, "Vec4 must be exactly 16 bytes");
// static_assert(sizeof(m2) == 16, "Mat2 must be exactly 16 bytes");
// static_assert(sizeof(m3) == 36, "Mat3 must be exactly 36 bytes");
static_assert(sizeof(m4) == 64, "Mat4 must be exactly 64 bytes");

#endif // !ARM_COMMON_H