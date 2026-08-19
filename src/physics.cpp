#include "physics.h"

u8 check_sphere_intersection(v3 origin, v3 direction, v3 center, f32 radius) {
    v3 otc = origin - center;

    f32 b = dot(otc, direction);
    f32 c = dot(otc, otc) - (radius * radius);

    f32 discriminant = (b * b) - c;
    if (discriminant < 0.0f) return 0;

    return ((-b - sqrt(discriminant)) > 0.0f);
}