#ifndef ARM_PHYSICS_H
#define ARM_PHYSICS_H

#include "common.h"

// enum class PhysicsStatus : u8 {};

typedef struct {
    v3 pos, direction;
} Ray_t;

typedef struct {
    v3 min, max;
    u8 mask;
} aabb_collider_t;

u8 check_aabb_collision(v3 origin, v3 direction, aabb_collider_t collider);

typedef struct {
    v3 pos;
} obb_collider_t;

u8 check_sphere_intersection(v3 origin, v3 direction, v3 center, f32 radius);

#endif // !ARM_PHYSICS_H