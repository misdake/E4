#include "Mat4.h"

#include <cstring>
#include <cmath>

const float MATRIX4_IDENTITY[]{
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
};

void E4::Mat4::set(const float* array) {
    memcpy(this, array, sizeof(Mat4));
}

void E4::Mat4::set(const Mat4& matrix) {
    memcpy(this, &matrix, sizeof(Mat4));
}

void E4::Mat4::set(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33) {
    m00 = n00;
    m10 = n10;
    m20 = n20;
    m30 = n30;
    m01 = n01;
    m11 = n11;
    m21 = n21;
    m31 = n31;
    m02 = n02;
    m12 = n12;
    m22 = n22;
    m32 = n32;
    m03 = n03;
    m13 = n13;
    m23 = n23;
    m33 = n33;
}

void E4::Mat4::setZero() {
    memset(this, 0, sizeof(Mat4));
}

void E4::Mat4::setIdentity() {
    memcpy(this, MATRIX4_IDENTITY, sizeof(Mat4));
}

void E4::Mat4::setTranslate(float x, float y, float z) {
    setIdentity();
    m30 = x;
    m31 = y;
    m32 = z;
}

void E4::Mat4::setRotateX(float r) {
    setIdentity();
    float ca = std::cos(r);
    float sa = std::sin(r);
    m11 = ca;
    m12 = sa;
    m21 = -sa;
    m22 = ca;
}

void E4::Mat4::setRotateY(float r) {
    setIdentity();
    float ca = std::cos(r);
    float sa = std::sin(r);
    m00 = ca;
    m02 = -sa;
    m20 = sa;
    m22 = ca;
}

void E4::Mat4::setRotateZ(float r) {
    setIdentity();
    float ca = std::cos(r);
    float sa = std::sin(r);
    m00 = ca;
    m01 = sa;
    m10 = -sa;
    m11 = ca;
}

void E4::Mat4::setRotate(float x, float y, float z, float r) {
    //TODO

}

void E4::Mat4::setScale(float s) {
    setZero();
    m00 = s;
    m11 = s;
    m22 = s;
    m33 = 1;
}

void E4::Mat4::setScale(float x, float y, float z) {
    setZero();
    m00 = x;
    m11 = y;
    m22 = z;
    m33 = 1;
}

void E4::Mat4::setTRS(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) {
    Mat4 temp1{}, temp2{}, temp3{};

    temp1.setTranslate(x, y, z);
    temp2.setScale(sx, sy, sz);
    multiply(temp1, temp2, temp3);

    this->setRotateY(ry);
    multiply(temp3, *this, temp1);
    temp2.setRotateX(rx);
    multiply(temp1, temp2, temp3);
    temp1.setRotateZ(rz);
    multiply(temp3, temp1, *this);
}

float* E4::Mat4::array() {
    return reinterpret_cast<float*>(this);
}

const float* E4::Mat4::array() const {
    return reinterpret_cast<const float*>(this);
}

float* E4::Mat4::operator[](int i) {
    return &array()[i * 4];
}

float E4::Mat4::determinant() const {
    return (m00 * m11 - m01 * m10) * (m22 * m33 - m23 * m32) - (m00 * m12 - m02 * m10) * (m21 * m33 - m23 * m31)
           + (m00 * m13 - m03 * m10) * (m21 * m32 - m22 * m31) + (m01 * m12 - m02 * m11) * (m20 * m33 - m23 * m30)
           - (m01 * m13 - m03 * m11) * (m20 * m32 - m22 * m30) + (m02 * m13 - m03 * m12) * (m20 * m31 - m21 * m30);
}

bool E4::Mat4::invert() {
    float s = determinant();
    if (s == 0.0f) {
        return false;
    }
    s = 1.0f / s;
    set((m11 * (m22 * m33 - m23 * m32) + m12 * (m23 * m31 - m21 * m33) + m13 * (m21 * m32 - m22 * m31)) * s,
        (m21 * (m02 * m33 - m03 * m32) + m22 * (m03 * m31 - m01 * m33) + m23 * (m01 * m32 - m02 * m31)) * s,
        (m31 * (m02 * m13 - m03 * m12) + m32 * (m03 * m11 - m01 * m13) + m33 * (m01 * m12 - m02 * m11)) * s,
        (m01 * (m13 * m22 - m12 * m23) + m02 * (m11 * m23 - m13 * m21) + m03 * (m12 * m21 - m11 * m22)) * s,
        (m12 * (m20 * m33 - m23 * m30) + m13 * (m22 * m30 - m20 * m32) + m10 * (m23 * m32 - m22 * m33)) * s,
        (m22 * (m00 * m33 - m03 * m30) + m23 * (m02 * m30 - m00 * m32) + m20 * (m03 * m32 - m02 * m33)) * s,
        (m32 * (m00 * m13 - m03 * m10) + m33 * (m02 * m10 - m00 * m12) + m30 * (m03 * m12 - m02 * m13)) * s,
        (m02 * (m13 * m20 - m10 * m23) + m03 * (m10 * m22 - m12 * m20) + m00 * (m12 * m23 - m13 * m22)) * s,
        (m13 * (m20 * m31 - m21 * m30) + m10 * (m21 * m33 - m23 * m31) + m11 * (m23 * m30 - m20 * m33)) * s,
        (m23 * (m00 * m31 - m01 * m30) + m20 * (m01 * m33 - m03 * m31) + m21 * (m03 * m30 - m00 * m33)) * s,
        (m33 * (m00 * m11 - m01 * m10) + m30 * (m01 * m13 - m03 * m11) + m31 * (m03 * m10 - m00 * m13)) * s,
        (m03 * (m11 * m20 - m10 * m21) + m00 * (m13 * m21 - m11 * m23) + m01 * (m10 * m23 - m13 * m20)) * s,
        (m10 * (m22 * m31 - m21 * m32) + m11 * (m20 * m32 - m22 * m30) + m12 * (m21 * m30 - m20 * m31)) * s,
        (m20 * (m02 * m31 - m01 * m32) + m21 * (m00 * m32 - m02 * m30) + m22 * (m01 * m30 - m00 * m31)) * s,
        (m30 * (m02 * m11 - m01 * m12) + m31 * (m00 * m12 - m02 * m10) + m32 * (m01 * m10 - m00 * m11)) * s,
        (m00 * (m11 * m22 - m12 * m21) + m01 * (m12 * m20 - m10 * m22) + m02 * (m10 * m21 - m11 * m20)) * s);
    return true;
}

bool E4::Mat4::invert(Mat4& dest) const {
    float s = determinant();
    if (s == 0.0f) {
        return false;
    }
    s = 1.0f / s;

    dest.m00 = (m11 * (m22 * m33 - m23 * m32) + m12 * (m23 * m31 - m21 * m33) + m13 * (m21 * m32 - m22 * m31)) * s;
    dest.m01 = (m21 * (m02 * m33 - m03 * m32) + m22 * (m03 * m31 - m01 * m33) + m23 * (m01 * m32 - m02 * m31)) * s;
    dest.m02 = (m31 * (m02 * m13 - m03 * m12) + m32 * (m03 * m11 - m01 * m13) + m33 * (m01 * m12 - m02 * m11)) * s;
    dest.m03 = (m01 * (m13 * m22 - m12 * m23) + m02 * (m11 * m23 - m13 * m21) + m03 * (m12 * m21 - m11 * m22)) * s;
    dest.m10 = (m12 * (m20 * m33 - m23 * m30) + m13 * (m22 * m30 - m20 * m32) + m10 * (m23 * m32 - m22 * m33)) * s;
    dest.m11 = (m22 * (m00 * m33 - m03 * m30) + m23 * (m02 * m30 - m00 * m32) + m20 * (m03 * m32 - m02 * m33)) * s;
    dest.m12 = (m32 * (m00 * m13 - m03 * m10) + m33 * (m02 * m10 - m00 * m12) + m30 * (m03 * m12 - m02 * m13)) * s;
    dest.m13 = (m02 * (m13 * m20 - m10 * m23) + m03 * (m10 * m22 - m12 * m20) + m00 * (m12 * m23 - m13 * m22)) * s;
    dest.m20 = (m13 * (m20 * m31 - m21 * m30) + m10 * (m21 * m33 - m23 * m31) + m11 * (m23 * m30 - m20 * m33)) * s;
    dest.m21 = (m23 * (m00 * m31 - m01 * m30) + m20 * (m01 * m33 - m03 * m31) + m21 * (m03 * m30 - m00 * m33)) * s;
    dest.m22 = (m33 * (m00 * m11 - m01 * m10) + m30 * (m01 * m13 - m03 * m11) + m31 * (m03 * m10 - m00 * m13)) * s;
    dest.m23 = (m03 * (m11 * m20 - m10 * m21) + m00 * (m13 * m21 - m11 * m23) + m01 * (m10 * m23 - m13 * m20)) * s;
    dest.m30 = (m10 * (m22 * m31 - m21 * m32) + m11 * (m20 * m32 - m22 * m30) + m12 * (m21 * m30 - m20 * m31)) * s;
    dest.m31 = (m20 * (m02 * m31 - m01 * m32) + m21 * (m00 * m32 - m02 * m30) + m22 * (m01 * m30 - m00 * m31)) * s;
    dest.m32 = (m30 * (m02 * m11 - m01 * m12) + m31 * (m00 * m12 - m02 * m10) + m32 * (m01 * m10 - m00 * m11)) * s;
    dest.m33 = (m00 * (m11 * m22 - m12 * m21) + m01 * (m12 * m20 - m10 * m22) + m02 * (m10 * m21 - m11 * m20)) * s;
    return true;
}

void E4::Mat4::transpose() {
    set(m00, m10, m20, m30,
        m01, m11, m21, m31,
        m02, m12, m22, m32,
        m03, m13, m23, m33);
}

void E4::Mat4::transpose(Mat4& dest) const {
    dest.m00 = m00;
    dest.m01 = m10;
    dest.m02 = m20;
    dest.m03 = m30;
    dest.m10 = m01;
    dest.m11 = m11;
    dest.m12 = m21;
    dest.m13 = m31;
    dest.m20 = m02;
    dest.m21 = m12;
    dest.m22 = m22;
    dest.m23 = m32;
    dest.m30 = m03;
    dest.m31 = m13;
    dest.m32 = m23;
    dest.m33 = m33;
}

void multiplySlow(const E4::Mat4& left, const E4::Mat4& right, E4::Mat4& dest) {
    dest.set(left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03,
             left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03,
             left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03,
             left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03,
             left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13,
             left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m13,
             left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13,
             left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13,
             left.m00 * right.m20 + left.m10 * right.m21 + left.m20 * right.m22 + left.m30 * right.m23,
             left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23,
             left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23,
             left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23,
             left.m00 * right.m30 + left.m10 * right.m31 + left.m20 * right.m32 + left.m30 * right.m33,
             left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33,
             left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33,
             left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33);
}

void multiplyFast(const E4::Mat4& left, const E4::Mat4& right, E4::Mat4& dest) {
    dest.m00 = left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03;
    dest.m01 = left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03;
    dest.m02 = left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03;
    dest.m03 = left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03;
    dest.m10 = left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13;
    dest.m11 = left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m13;
    dest.m12 = left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13;
    dest.m13 = left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13;
    dest.m20 = left.m00 * right.m20 + left.m10 * right.m21 + left.m20 * right.m22 + left.m30 * right.m23;
    dest.m21 = left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23;
    dest.m22 = left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23;
    dest.m23 = left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23;
    dest.m30 = left.m00 * right.m30 + left.m10 * right.m31 + left.m20 * right.m32 + left.m30 * right.m33;
    dest.m31 = left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33;
    dest.m32 = left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33;
    dest.m33 = left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33;
}

void E4::Mat4::multiply(const Mat4& left, const Mat4& right, Mat4& dest) {
    if (&left != &dest && &right != &dest) {
        multiplyFast(left, right, dest);
    } else {
        multiplySlow(left, right, dest);
    }
}

E4::Mat4 E4::Mat4::operator*(const Mat4& right) const {
    Mat4 result{};
    multiply(*this, right, result);
    return result;
}

E4::Vec3 E4::Mat4::transformPoint(const E4::Vec3& src) const {
    Vec3 dst{};
    dst.x = m00 * src.x + m10 * src.y + m20 * src.z + m30;
    dst.y = m01 * src.x + m11 * src.y + m21 * src.z + m31;
    dst.z = m02 * src.x + m12 * src.y + m22 * src.z + m32;
    float w = m03 * src.x + m13 * src.y + m23 * src.z + m33;
    float iw = 1.f / w;
    dst.x *= iw;
    dst.y *= iw;
    dst.z *= iw;
    return dst;
}

E4::Vec3 E4::Mat4::transformNormal(const E4::Vec3& src) const {
    Mat4 m{};
    invert(m);
    m.transpose();

    Vec3 dst{};
    dst.x = m00 * src.x + m10 * src.y + m20 * src.z;
    dst.y = m01 * src.x + m11 * src.y + m21 * src.z;
    dst.z = m02 * src.x + m12 * src.y + m22 * src.z;
    dst.normalize();
    return dst;
}

void E4::Mat4::setPerspective(float fovy, float aspect, float zNear, float zFar) {
    float ymax = zNear * std::tan(fovy);
    float ymin = -ymax;
    float xmin = ymin * aspect;
    float xmax = ymax * aspect;

    setIdentity();
    m00 = (2.0f * zNear) / (xmax - xmin);
    m11 = (2.0f * zNear) / (ymax - ymin);
    m22 = -(zFar + zNear) / (zFar - zNear);
    m20 = (xmax + xmin) / (xmax - xmin);
    m21 = (ymax + ymin) / (ymax - ymin);
    m23 = -1.0;
    m32 = -(2.0f * zFar * zNear) / (zFar - zNear);
}

void E4::Mat4::setOrtho(float l, float r, float t, float b, float n, float f) {
    m00 = 2 / (r - l);
    m01 = 0;
    m02 = 0;
    m03 = 0;

    m10 = 0;
    m11 = 2 / (t - b);
    m12 = 0;
    m13 = 0;

    m20 = 0;
    m21 = 0;
    m22 = -2 / (f - n);
    m23 = 0;

    m30 = -(r + l) / (r - l);
    m31 = -(t + b) / (t - b);
    m32 = -(f + n) / (f - n);
    m33 = 1;
}
