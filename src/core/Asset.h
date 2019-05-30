#pragma once

#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdint>
#include <limits>
#include <iostream> //TODO

namespace E4 {

    class Asset {

    };

    template<typename T>
    class AssetPool;

    template<typename T>
    class AssetPointer;

    template<typename T>
    class AssetPointer {
    private:
        friend class AssetPool<T>;

        AssetPool<T>* pool;
        uint32_t index;

        AssetPointer(AssetPool<T>& pool, uint32_t index) : pool(&pool), index(index) {}

    public:
        static const uint32_t MAX_INDEX = std::numeric_limits<uint32_t>::max();

        AssetPointer() : pool(nullptr), index(MAX_INDEX) {}
        AssetPointer(const AssetPointer& other) : pool(other.pool), index(other.index) {}
        AssetPointer& operator=(const AssetPointer& other) {
            pool = other.pool;
            index = other.index;
            return *this;
        }

        T* operator->() { return &get(); }
        const T* operator->() const { return &get(); }

        T& get();
        const T& get() const;

        void free();
    };

    template<typename T>
    class AssetPool {
    private:
        friend class AssetPointer<T>;

        std::vector<T> array;
        std::deque<uint32_t> empty;

    public:
        AssetPointer<T> alloc() {
            uint32_t index = 0;
            if (empty.empty()) {
                array.emplace_back();
                index = array.size() - 1;
            } else {
                index = empty.back();
                empty.pop_back();
            }
            return AssetPointer<T>(*this, index);
        }

        void shrintToFit() {
            array.shrink_to_fit();
        }
    };

    template<typename T>
    T& AssetPointer<T>::get() {
        if (index < MAX_INDEX && index < pool->array.size()) {
            return pool->array[index];
        } else {
            //TODO error
            std::cout << "access an invalid pointer" << std::endl;
            return pool->array[index];
        }
    }

    template<typename T>
    const T& AssetPointer<T>::get() const {
        if (index < MAX_INDEX && index < pool->array.size()) {
            return pool->array[index];
        } else {
            //TODO error
            std::cout << "access an invalid pointer" << std::endl;
            return pool->array[index];
        }
    }

    template<typename T>
    void AssetPointer<T>::free() {
        if (index < MAX_INDEX && index < pool->array.size()) {
            pool->empty.push_back(index);
            index = MAX_INDEX;
        } else {
            //TODO error
            std::cout << "free an invalid pointer" << std::endl;
        }
    }

}
