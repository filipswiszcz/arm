#ifndef ARM_PHYSICS_H
#define ARM_PHYSICS_H

#include "common.h"
#include "math.h"

// enum class PhysicsStatus : u8 {};

u8 check_sphere_intersection(v3 origin, v3 direction, v3 center, f32 radius);

#endif // !ARM_PHYSICS_H