#include "math.h"

// FLOAT

float radians(float degrees) {
    return degrees * 0.01745329251994329576923690768489;
}

/*float sin(float v) {
    int x = 14;
    
    // del s
    std::cout << "x=" << x << std::endl;
    __asm__(
        "movl %1, %%eax\n"
        "addl $1, %%eax\n"
        "movl %%eax, %0\n"
        : "=r" (x)
        : "r" (x)
        : "%eax"
    );
    std::cout << "x=" << x << std::endl;
    // del e

    return 1.0f;
}*/

// float cos(float v) {}

// float tan(float v) {}

/*float q_tan(float v) { // blows up near +-PI/2 (90 or -90 degrees)
    float pisqby4 = 2.4674011002723397f;
    float om8bypisq = 0.1894305308612978f;
    float vsq = v * v;
    return v * (pisqby4 - om8bypisq * vsq) / (pisqby4 - vsq);
}*/

/*float q_rsqrt(float v) {
    long i;
    float x2, y;

    x2 = v * 0.5f;
    y = v;
    i = *(long*) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*) &i;
    y = y * (1.5f - (x2 * y * y));
    
    return y;
}*/

// VECTOR

// Vec3_t normalize(const Vec3_t v) {
//     float l = v.x * v.x + v.y * v.y + v.z * v.z;
//     float rl = q_rsqrt(l);
//     return {v.x * rl, v.y * rl, v.z * rl};
// }

Vec3_t normalize(const Vec3_t v) {
    float l = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    if (l < 0.00001f) return {0.0f, 0.0f, 0.0f};
    return {v.x / l, v.y / l, v.z / l};
}

Vec3_t cross(const Vec3_t a, const Vec3_t b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float dot(const Vec3_t a, const Vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float determinant3v(Vec3_t a, Vec3_t b, Vec3_t c) {
    return a.x * (b.y * c.z - b.z * c.y) - a.y * (b.x * c.z - b.z * c.x) + a.z * (b.x * c.y - b.y * c.x);
}

double length(const Vec3_t v) {
    return sqrt(dot(v, v));
}

// MATRIX

/*static void _read_mat4(Mat4_t m) {
    for (int i = 0; i < 4; i++) { // fun fact: cpu registers are often 32 or 64-bit wide and the loop control variables are almost always stored in CPU registers
        std::cout << "[";
        for (int j = 0; j < 4; j++) {
            std::cout << std::fixed << std::setw(7) << std::setprecision(2) << m.m[i][j];
            if (j < 3) std::cout << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}*/

Mat4_t orthographic(float l, float r, float b, float t, float znear, float zfar) {
    Mat4_t res = Mat4(0);
    res.m[0][0] = 2.0f / (r - l);
    res.m[1][1] = 2.0f / (t - b);
    res.m[2][2] = -2.0f / (zfar / znear);
    res.m[3][0] = -(r + l) / (r - l);
    res.m[3][1] = -(t + b) / (t - b);
    res.m[3][2] = -(zfar + znear) / (zfar - znear);
    res.m[3][3] = 1.0f;
    return res;
}

Mat4_t perspective(float fovy, float aspect, float znear, float zfar) {
    float rad = fovy;
    float tan_half_fovy = tan(rad / 2.0f);

    Mat4_t res = Mat4(0.0f);
    res.m[0][0] = 1.0f / (aspect * tan_half_fovy);
    res.m[1][1] = 1.0f / tan_half_fovy;
    res.m[2][2] = -(zfar + znear) / (zfar - znear);
    // res.m[2][2] = (zfar + znear) / (znear - zfar);
    res.m[2][3] = -1.0f;
    res.m[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
    // res.m[3][2] = (2.0f * zfar * znear) / (znear - zfar);
    res.m[3][3] = 0.0f;

    return res;
}

Mat4_t translate(Mat4_t m, Vec3_t v) {
    Mat4_t res = m;
    // res.m[3][0] = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0];
    // res.m[3][1] = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1];
    // res.m[3][2] = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2];
    // res.m[3][3] = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3];
    res.m[3][0] += v.x;
    res.m[3][1] += v.y;
    res.m[3][2] += v.z;
    return res;
}

Mat4_t rotate(Mat4_t m, float angle, Vec3_t v) {
    float const a = radians(angle);
    float const c = cos(a);
    float const s = sin(a);

    Vec3_t axis = normalize(v);

    Mat4_t rot = Mat4(0);
    rot.m[0][0] = c + (1 - c) * axis.x * axis.x;
    rot.m[0][1] = (1 - c) * axis.x * axis.y - s * axis.z;
    rot.m[0][2] = (1 - c) * axis.x * axis.z + s * axis.y;

    rot.m[1][0] = (1 - c) * axis.y * axis.x + s * axis.z;
    rot.m[1][1] = c + (1 - c) * axis.y * axis.y;
    rot.m[1][2] = (1 - c) * axis.y * axis.z - s * axis.x;

    rot.m[2][0] = (1 - c) * axis.z * axis.x - s * axis.y;
    rot.m[2][1] = (1 - c) * axis.z * axis.y + s * axis.x;
    rot.m[2][2] = c + (1 - c) * axis.z * axis.z;

    rot.m[3][3] = 1.0f;

    Mat4_t res = Mat4(0);
    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            res.m[i][j] = 0;
            for (uint32_t k = 0; k < 4; k++) {
                res.m[i][j] += m.m[i][k] * rot.m[k][j];
            }
        }
    }

    return res;
}

Mat4_t look_at(Vec3_t position, Vec3_t target, Vec3_t head) {
    const Vec3_t t = normalize(target - position);
    const Vec3_t r = normalize(cross(t, head));
    const Vec3_t u = normalize(cross(r, t));

    Mat4_t result = Mat4(1.0f);
    result.m[0][0] = r.x;
    result.m[1][0] = r.y;
    result.m[2][0] = r.z;

    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;
    
    result.m[0][2] = -t.x;
    result.m[1][2] = -t.y;
    result.m[2][2] = -t.z;
    result.m[3][0] = -dot(r, position);
    result.m[3][1] = -dot(u, position);
    result.m[3][2] = dot(t, position);
    result.m[3][3] = 1.0f;

    return result;
}

Mat4_t inverse(Mat4_t m) {
    float d = determinant(m);
    
    Mat4_t res(0.0f);
    if (std::abs(d) < 1e-8f) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res.m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
        return res;
    }

    float id = 1.0f / d;
    res.m[0][0] = determinant3v(Vec3(m.m[1][1], m.m[1][2], m.m[1][3]), Vec3(m.m[2][1], m.m[2][2], m.m[2][3]), Vec3(m.m[3][1], m.m[3][2], m.m[3][3])) * id;
    res.m[0][1] = -determinant3v(Vec3(m.m[1][0], m.m[1][2], m.m[1][3]), Vec3(m.m[2][0], m.m[2][2], m.m[2][3]), Vec3(m.m[3][0], m.m[3][2], m.m[3][3])) * id;
    res.m[0][2] = determinant3v(Vec3(m.m[1][0], m.m[1][1], m.m[1][3]), Vec3(m.m[2][0], m.m[2][1], m.m[2][3]), Vec3(m.m[3][0], m.m[3][1], m.m[3][3])) * id;
    res.m[0][3] = -determinant3v(Vec3(m.m[1][0], m.m[1][1], m.m[1][2]), Vec3(m.m[2][0], m.m[2][1], m.m[2][2]), Vec3(m.m[3][0], m.m[3][1], m.m[3][2])) * id;
    res.m[1][0] = -determinant3v(Vec3(m.m[0][1], m.m[0][2], m.m[0][3]), Vec3(m.m[2][1], m.m[2][2], m.m[2][3]), Vec3(m.m[3][1], m.m[3][2], m.m[3][3])) * id;
    res.m[1][1] = determinant3v(Vec3(m.m[0][0], m.m[0][2], m.m[0][3]), Vec3(m.m[2][0], m.m[2][2], m.m[2][3]), Vec3(m.m[3][0], m.m[3][2], m.m[3][3])) * id;
    res.m[1][2] = -determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][3]), Vec3(m.m[2][0], m.m[2][1], m.m[2][3]), Vec3(m.m[3][0], m.m[3][1], m.m[3][3])) * id;
    res.m[1][3] = determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][2]), Vec3(m.m[2][0], m.m[2][1], m.m[2][2]), Vec3(m.m[3][0], m.m[3][1], m.m[3][3])) * id;
    res.m[2][0] = determinant3v(Vec3(m.m[0][1], m.m[0][2], m.m[0][3]), Vec3(m.m[1][1], m.m[1][2], m.m[1][3]), Vec3(m.m[3][1], m.m[3][2], m.m[3][3])) * id;
    res.m[2][1] = -determinant3v(Vec3(m.m[0][0], m.m[0][2], m.m[0][3]), Vec3(m.m[1][0], m.m[1][2], m.m[1][3]), Vec3(m.m[3][0], m.m[3][2], m.m[3][3])) * id;
    res.m[2][2] = determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][3]), Vec3(m.m[1][0], m.m[1][1], m.m[1][3]), Vec3(m.m[3][0], m.m[3][1], m.m[3][3])) * id;
    res.m[2][3] = -determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][2]), Vec3(m.m[1][0], m.m[1][1], m.m[1][2]), Vec3(m.m[3][0], m.m[3][1], m.m[3][2])) * id;
    res.m[3][0] = -determinant3v(Vec3(m.m[0][1], m.m[0][2], m.m[0][3]), Vec3(m.m[1][1], m.m[1][2], m.m[1][3]), Vec3(m.m[2][1], m.m[2][2], m.m[2][3])) * id;
    res.m[3][1] = determinant3v(Vec3(m.m[0][0], m.m[0][2], m.m[0][3]), Vec3(m.m[1][0], m.m[1][1], m.m[1][3]), Vec3(m.m[2][0], m.m[2][2], m.m[2][3])) * id;
    res.m[3][2] = -determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][3]), Vec3(m.m[1][0], m.m[1][1], m.m[1][3]), Vec3(m.m[2][0], m.m[2][1], m.m[2][3])) * id;
    res.m[3][3] = determinant3v(Vec3(m.m[0][0], m.m[0][1], m.m[0][2]), Vec3(m.m[1][0], m.m[1][1], m.m[1][2]), Vec3(m.m[2][0], m.m[2][1], m.m[2][2])) * id;

    return res;
}

float determinant(Mat4_t m) {
    float d = 0.0f;
    d += m.m[0][0] * determinant3v(Vec3(m.m[1][1], m.m[1][2], m.m[1][3]), Vec3(m.m[2][1], m.m[2][2], m.m[2][3]), Vec3(m.m[3][1], m.m[3][2], m.m[3][3]));
    d -= m.m[0][1] * determinant3v(Vec3(m.m[1][0], m.m[1][2], m.m[1][3]), Vec3(m.m[2][0], m.m[2][2], m.m[2][3]), Vec3(m.m[3][0], m.m[3][2], m.m[3][3]));
    d += m.m[0][2] * determinant3v(Vec3(m.m[1][0], m.m[1][1], m.m[1][3]), Vec3(m.m[2][0], m.m[2][1], m.m[2][3]), Vec3(m.m[3][0], m.m[3][1], m.m[3][3]));
    d -= m.m[0][3] * determinant3v(Vec3(m.m[1][0], m.m[1][1], m.m[1][2]), Vec3(m.m[2][0], m.m[2][1], m.m[2][2]), Vec3(m.m[3][0], m.m[3][1], m.m[3][2]));
    return d;
}

Vec3_t project(const Vec3_t v, const Mat4_t view, const Mat4_t projection, const Vec4_t viewport) {
    Vec4_t clip = projection * view * Vec4(v.x, v.y, v.z, 1.0f);

    if (clip.w != 0.0f) {
        clip.x /= clip.w;
        clip.y /= clip.w;
        clip.z /= clip.w;
    }

    Vec3_t screen;
    screen.x = viewport.x + (clip.x + 1.0f) * viewport.z / 2.0f;
    screen.y = viewport.y + (clip.y + 1.0f) * viewport.w / 2.0f;
    screen.z = clip.z;

    return screen;
}

Vec3_t unproject(const Vec3_t v, const Mat4_t view, const Mat4_t projection, const Vec4_t viewport) {
    Mat4_t inv = inverse(projection * view);

    Vec4_t temporary = Vec4(v.x, v.y, v.z, 1.0f);
    temporary.x = (temporary.x - viewport.x) / viewport.z * 2.0f - 1.0f;
    temporary.y = (temporary.y - viewport.y) / viewport.w * 2.0f - 1.0f;
    temporary.z = temporary.z * 2.0f - 1.0f;

    Vec4_t object = inv * temporary;
    if (object.w != 0.0f) {
        object /= object.w;
    }

    return {object.x, object.y, object.z};
}

// QUATERNION

Mat4_t rotate_quat(Mat4_t m, Quat_t q) {}