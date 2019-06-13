#pragma once

#include "Vec3.h"

namespace E4 {

    class Mat4 {
    public:
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;

        //set
        void set(const float* array);
        void set(const Mat4& matrix);
        void set(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
        void setZero();
        void setIdentity();
        //TODO add Vec3 versions
        void setTranslate(float x, float y, float z);
        void setRotateX(float r);
        void setRotateY(float r);
        void setRotateZ(float r);
        void setRotate(float x, float y, float z, float r);
        void setScale(float s);
        void setScale(float x, float y, float z);
        void setTRS(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz);
        //TODO
//    void setPerspective(float fovy, float aspect, float zNear, float zFar);
//    void setOrtho(float left, float right, float top, float bottom, float zNear, float zFar);
//    void setLookAt(float fx, float fy, float fz, float tx, float ty, float tz, float ux, float uy, float uz);

        //get
        float* array();
        const float* array() const;
        float* operator[](int i);

        //op
        Mat4& operator=(const Mat4& src) = default;
        float determinant() const;
        bool invert();
        bool invert(Mat4& dest) const;
        void transpose();
        void transpose(Mat4& dest) const;

        static void multiply(const Mat4& left, const Mat4& right, Mat4& dest);
        Mat4 operator*(const Mat4& right) const;

        Vec3 operator*(const Vec3& src) const;

    };

}