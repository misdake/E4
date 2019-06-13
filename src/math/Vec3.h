#pragma once

namespace E4 {

    class Vec3 {
    public:
        float x, y, z;

        //init
        Vec3() = default;
        Vec3(const Vec3& vector);
        Vec3(float x, float y, float z);

        //set
        void set(float x, float y, float z);

        //get
        float* array();
        const float* array() const;

        void normalize();

        float length() const;
    };

}