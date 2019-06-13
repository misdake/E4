#include "Vec3.h"

#include <cmath>

E4::Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

E4::Vec3::Vec3(const Vec3& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

void E4::Vec3::set(float tx, float ty, float tz) {
    x = tx;
    y = ty;
    z = tz;
}

float* E4::Vec3::array() {
    return reinterpret_cast<float*>(this);
}

const float* E4::Vec3::array() const {
    return reinterpret_cast<const float*>(this);
}

void E4::Vec3::normalize() {
    float len = length();
    if (len > 0) {
        float s = 1.0f / len;
        x *= s;
        y *= s;
        z *= s;
    }
}

float E4::Vec3::length() const {
    float t = x * x + y * y + z * z;
    float length = std::sqrt(t);
    return length;
}