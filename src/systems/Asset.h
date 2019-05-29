#pragma once

#include <vector>
#include <stack>
#include <cstdint>

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

        AssetPool<T>& pool;
        int index;

        AssetPointer(AssetPool<T>& pool, int index) : pool(pool), index(index) {}

    public:
        T& get() { return pool.array[index]; }

        const T& get() const { return pool.array[index]; }
    };

    template<typename T>
    class AssetPool {
    private:
        friend class AssetPointer<T>;

        std::vector<T> array;
        std::stack<uint32_t> empty;

    public:
        AssetPointer<T> alloc() {
            //TODO find empty and reuse it
            //TODO memset?
            array.emplace_back();
            return AssetPointer<T>(*this, array.size() - 1);
        }

        void tryShrink() {
            array.shrink_to_fit();
        }

        void free(AssetPointer<T>& pointer) {
            //TODO
        }
    };

}
