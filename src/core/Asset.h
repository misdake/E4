#pragma once

#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdint>
#include <limits>
#include <map>
#include <iostream> //TODO

namespace E4 {

    template<typename T>
    class AssetLoader;

    template<typename T>
    class AssetPool;

    template<typename T>
    class Asset;

    template<typename T>
    class Asset {
    private:
        friend class AssetPool<T>;

        friend class AssetLoader<T>;

        AssetPool<T>* pool;
        uint32_t index;

        Asset(AssetPool<T>& pool, uint32_t index) : pool(&pool), index(index) {}

    public:
        static const uint32_t MAX_INDEX = std::numeric_limits<uint32_t>::max();

        Asset() : pool(nullptr), index(MAX_INDEX) {}

        T* operator->() { return &get(); }
        const T* operator->() const { return &get(); }

        bool valid() const;
        T& get();
        const T& get() const;

        const std::string& name() const;
    };

    template<typename T>
    class AssetPool {
    protected:
        friend class Asset<T>;

        std::vector<T> array;
        std::deque<uint32_t> empty;

    public:
        Asset<T> alloc() {
            uint32_t index = 0;
            if (empty.empty()) {
                array.emplace_back();
                index = array.size() - 1;
            } else {
                index = empty.back();
                empty.pop_back();
            }
            return Asset<T>(*this, index);
        }

        void free(const Asset<T>& p) {
            if (p.index < Asset<T>::MAX_INDEX && p.index < array.size()) {
                empty.push_back(p.index);
                p.index = Asset<T>::MAX_INDEX;
            } else {
                //TODO error
                std::cout << "free an invalid pointer" << std::endl;
            }
        }
    };

    template<typename T>
    class AssetLoader : private AssetPool<T> {
    protected:
        friend class Asset<T>;

        std::vector<std::string> names;
        std::map<std::string, uint32_t> map;

        Asset<T> alloc(const std::string& name) {
            uint32_t index = 0;
            if (AssetPool<T>::empty.empty()) {
                AssetPool<T>::array.emplace_back();
                names.emplace_back(name);
                index = AssetPool<T>::array.size() - 1;
            } else {
                index = AssetPool<T>::empty.back();
                AssetPool<T>::empty.pop_back();
                names[index] = name;
            }
            if (!name.empty()) {
                map.insert(std::pair(name, index));
            }
            return Asset<T>(*this, index);
        }
    public:
        Asset<T> get(const std::string& name) {
            auto iter = map.find(name);
            if (iter == map.end()) {
                Asset<T> t = alloc(name);
                t->load(name);
                return t;
            } else {
                uint32_t index = iter->second;
                return Asset<T>(*this, index);
            }
        }
        void freeLoaded(const std::string& name) {
            auto iter = map.find(name);
            if (iter != map.end()) {
                uint32_t index = iter->second;
                Asset<T> p(*this, index);
                p->unload();
                AssetPool<T>::empty.push_back(p.index);
                map.erase(names[p.index]);
                names[p.index] = "";
            } else {
                //TODO error
                std::cout << "asset not found" << std::endl;
            }
        }
    };

    template<typename T>
    bool Asset<T>::valid() const {
        return index < MAX_INDEX && index < pool->array.size();
    }

    template<typename T>
    T& Asset<T>::get() {
        if (index < MAX_INDEX && index < pool->array.size()) {
            return pool->array[index];
        } else {
            //TODO error
            std::cout << "access an invalid pointer" << std::endl;
            return pool->array[index];
        }
    }

    template<typename T>
    const T& Asset<T>::get() const {
        if (index < MAX_INDEX && index < pool->array.size()) {
            return pool->array[index];
        } else {
            //TODO error
            std::cout << "access an invalid pointer" << std::endl;
            return pool->array[index];
        }
    }

    template<typename T>
    const std::string& Asset<T>::name() const {
        if (index < MAX_INDEX && index < pool->array.size()) {
            return pool->names[index];
        } else {
            //TODO error
            std::cout << "access an invalid pointer" << std::endl;
            return pool->names[index];
        }
    }

}
