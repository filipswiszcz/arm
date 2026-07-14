#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>

// FLOAT

float radians(float degrees);

/*float sin(float v);

float cos(float v);

float tan(float v);

float q_tan(float v);

float q_rsqrt(float v);*/

// VECTOR

#define Vec2(x, y) ((Vec2_t) {x, y})

typedef struct Vec2 {
    float x, y;

    Vec2 operator+(const Vec2 &o) const {
        return {x + o.x, y + o.y};
    }
    Vec2 &operator+=(const Vec2 &o) {
        return (x += o.x, y += o.y, *this);
    }
    Vec2 operator-(const Vec2 &o) const {
        return {x - o.x, y - o.y};
    }
    Vec2 &operator-=(const Vec2 &o) {
        return (x -= o.x, y -= o.y, *this);
    }
    Vec2 operator*(const float s) const {
        return {x * s, y * s};
    } // scalar should be type-free
    Vec2 &operator*=(const float s) {
        return (x *= s, y *= s, *this);
    }

} Vec2_t;

#define Vec3(x, y, z) ((Vec3_t) {x, y, z})

typedef struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3 &o) const {
        return {x + o.x, y + o.y, z + o.z};
    }
    Vec3 &operator+=(const Vec3 &o) {
        return (x += o.x, y += o.y, z += o.z, *this);
    }
    Vec3 operator-(const Vec3 &o) const {
        return {x - o.x, y - o.y, z - o.z};
    }
    Vec3 &operator-=(const Vec3 &o) {
        return (x -= o.x, y -= o.y, z -= o.z, *this);
    }
    Vec3 operator*(const float s) const {
        return {x * s, y * s, z * s};
    }
    Vec3 &operator*=(const float s) {
        return (x *= s, y *= s, z *= s, *this);
    }

} Vec3_t;

#define Vec4(x, y, z, w) ((Vec4_t) {x, y, z, w})

typedef struct Vec4 {
    float x, y, z, w;

    Vec4 operator+(const Vec4 &o) const {
        return {x + o.x, y + o.y, z + o.z, w + o.w};
    }
    Vec4 &operator+=(const Vec4 &o) {
        return (x += o.x, y += o.y, z += o.z, w += o.w, *this);
    }
    Vec4 operator-(const Vec4 &o) const {
        return {x - o.x, y - o.y, z - o.z, w - o.w};
    }
    Vec4 &operator-=(const Vec4 &o) {
        return (x -= o.x, y -= o.y, z -= o.z, w -= o.w, *this);
    }
    Vec4 operator*(const float s) const {
        return {x * s, y * s, z * s, w * s};
    }
    Vec4 &operator*=(const float s) {
        return (x *= s, y *= s, z *= s, w *= s, *this);
    }
    Vec4 operator/(const float s) const {
        return {x / s, y / s, z / s, w / s};
    }
    Vec4 &operator/=(const float s) {
        return (x / s, y / s, z / s, w / s, *this);
    }

} Vec4_t;

Vec3_t normalize(const Vec3_t v);

Vec3_t cross(const Vec3_t a, const Vec3_t b);

float dot(const Vec3_t a, const Vec3_t b);

float determinant3v(Vec3_t a, Vec3_t b, Vec3_t c);

double length(const Vec3_t v);

// MATRIX

typedef struct Mat4 {
    float m[4][4];

    explicit Mat4(float v) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? v : 0;
            }
        }
    }

    Mat4 operator*(const Mat4 &o) const {
        Mat4 r(0.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    r.m[i][j] += m[i][k] * o.m[k][j];
                }
            }
        }
        return r;
    }
    Mat4 &operator*=(const Mat4 &o) {
        *this = *this * o;
        return *this;
    }
    Mat4 operator/(const Mat4 &o) const {
        Mat4 r(0.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                r.m[i][j] = m[i][j] / o.m[i][j];
            }
        }
        return r;
    }
    Mat4 &operator/=(const Mat4 &o) {
        *this = *this / o;
        return *this;
    }
    
} Mat4_t;

inline Vec4_t operator*(const Mat4_t &m, const Vec4_t &o) {
    Vec4_t r;

    r.x = m.m[0][0] * o.x + m.m[0][1] * o.y + m.m[0][2] * o.z + m.m[0][3] * o.w;
    r.y = m.m[1][0] * o.x + m.m[1][1] * o.y + m.m[1][2] * o.z + m.m[1][3] * o.w;
    r.z = m.m[2][0] * o.x + m.m[2][1] * o.y + m.m[2][2] * o.z + m.m[2][3] * o.w;
    r.w = m.m[3][0] * o.x + m.m[3][1] * o.y + m.m[3][2] * o.z + m.m[3][3] * o.w;

    return r;
}

static void _read_mat4(Mat4_t m) {
    for (int i = 0; i < 4; i++) { // fun fact: cpu registers are of ten 32 or 64-bit wide and the loop control variables are almost always stored in CPU registers
        std::cout << "[";
        for (int j = 0; j < 4; j++) {
            // std::cout << std::fixed << ((width) ? std::setw(width) : std::setw(1)) << std::setprecision(6) << m.m[i][j]; it doesn't fucking work :(
            std::cout << std::fixed << std::setprecision(6) << m.m[i][j];
            if (j < 3) std::cout << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

Mat4_t orthographic(float l, float r, float b, float t, float znear, float zfar);

Mat4_t perspective(float fovy, float aspect, float znear, float zfar);

Mat4_t translate(Mat4_t m, Vec3_t v);

Mat4_t rotate(Mat4_t m, float angle, Vec3_t v);

Mat4_t look_at(Vec3_t pos, Vec3_t target, Vec3_t up);

Mat4_t inverse(Mat4_t m);

float determinant(Mat4_t m);

// vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport
// pointers should be used in args
Vec3_t unproject(const Vec3_t v, const Mat4_t model, const Mat4_t projection, const Vec4_t viewport);

// QUATERNION

typedef Vec4_t Quat_t;

Mat4_t rotate_quat(Mat4_t m, Quat_t q);

#endif // !__MATH_H_