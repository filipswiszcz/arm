#include "physics.h"

u8 check_aabb_collision(v3 pos, v3 direction, aabb_collider_t collider) {
    f32 tm0 = (collider.min.x - pos.x) / direction.x;
    f32 tm1 = (collider.max.x - pos.x) / direction.x;
    f32 tm2 = (collider.min.y - pos.y) / direction.y;
    f32 tm3 = (collider.max.y - pos.y) / direction.y;
    f32 tm4 = (collider.min.z - pos.z) / direction.z;
    f32 tm5 = (collider.max.z - pos.z) / direction.z;

    f32 tmin = std::max(std::max(std::min(tm0, tm1), std::min(tm2, tm3)), std::min(tm4, tm5));
    f32 tmax = std::min(std::min(std::max(tm0, tm1), std::max(tm2, tm3)), std::max(tm4, tm5));

    if (tmin < 0) return 1;
    if (tmax < 0) return 0;
    if (tmin > tmax) return 0;
    
    return 1;
}

u8 check_sphere_intersection(v3 origin, v3 direction, v3 center, f32 radius) {
    v3 otc = origin - center;

    f32 b = dot(otc, direction);
    f32 c = dot(otc, otc) - (radius * radius);

    f32 discriminant = (b * b) - c;
    if (discriminant < 0.0f) return 0;

    return ((-b - sqrt(discriminant)) > 0.0f);
}