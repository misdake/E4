#pragma once

#include <vector>
#include <stack>
#include <cstdint>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include "../util/Log.h"

namespace E4 {

    template<typename T>
    class AssetPool;

    template<typename T>
    class AssetLoader;

    template<typename T>
    class Asset {
    private:
        friend class AssetPool<T>;

        friend class AssetLoader<T>;

        AssetPool<T>* pool;
        uint32_t index;

        Asset(AssetPool<T>& pool, uint32_t index) : pool(&pool), index(index) {}

    public:
        Asset() : pool(nullptr), index(0) {}

        T* operator->() { return &get(); }
        const T* operator->() const { return &get(); }
        T& operator*() { return get(); }
        const T& operator*() const { return get(); }

        bool valid() const;
        T& get();
        const T& get() const;

        void free();
    };

    template<typename T>
    class AssetPool {
    protected:
        friend class Asset<T>;

        std::vector<T> array;
        std::vector<bool> bitmap;
        std::deque<uint32_t> empty;

    public:
        AssetPool() {
            array.emplace_back();
            bitmap.emplace_back(false);
        }

        Asset<T> alloc() {
            uint32_t index = 0;
            if (empty.empty()) {
                array.emplace_back();
                bitmap.emplace_back(false);
                index = array.size() - 1;
            } else {
                index = empty.back();
                empty.pop_back();
            }
            return Asset<T>(*this, index);
        }

        uint32_t size() {
            return array.size() - 1 - empty.size();
        }

        void free(const Asset<T>& p) {
            if (p.pool != this) {
                Log::error("it's not my pointer");
                return;
            }
            if (p.index != 0 && p.index < array.size()) {
                empty.push_back(p.index);
            } else {
                Log::error("free an invalid pointer");
            }
        }

        void clear() {
            array.clear();
            bitmap.clear();
            empty.clear();
            array.emplace_back();
            bitmap.emplace_back(false);
        }

        void checkBegin() {
            std::fill(bitmap.begin(), bitmap.end(), false);
            bitmap[0] = true;
            for (auto& e : empty) {
                bitmap[e] = true;
            }
        }
        void check(const Asset<T>& p) {
            if (p.pool != this) {
                Log::error("it's not my pointer");
                return;
            }
            if (p.index != 0 && p.index < array.size()) {
                bitmap[p.index] = true;
            } else {
                Log::error("free an invalid pointer");
            }
        }
        void checkEnd(std::function<void(Asset<T>&)>&& garbageCallback) {
            for (int i = 0; i < array.size(); i++) {
                if (!bitmap[i]) {
                    Asset<T> asset(*this, i);
                    garbageCallback(asset);
                }
            }
        }
    };

    template<typename T>
    class AssetLoader : private AssetPool<T> {
    protected:
        friend class Asset<T>;

        std::string folder;
        std::vector<std::string> names;
        std::map<std::string, uint32_t> map;

        Asset<T> alloc(const std::string& name) {
            uint32_t index = 0;
            if (AssetPool<T>::empty.empty()) {
                AssetPool<T>::array.emplace_back();
                AssetPool<T>::bitmap.emplace_back(false);
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
        explicit AssetLoader(std::string folder) :
            AssetPool<T>(),
            folder(std::move(folder)) {
            names.emplace_back("");
        }

        Asset<T> get(const std::string& name) {
            auto iter = map.find(name);
            if (iter == map.end()) {
                Asset<T> t = alloc(name);
                t->load(folder, name);
                return t;
            } else {
                uint32_t index = iter->second;
                return Asset<T>(*this, index);
            }
        }
        void freeLoaded(std::string name) {
            auto iter = map.find(name);
            if (iter != map.end()) {
                uint32_t index = iter->second;
                Asset<T> p(*this, index);
                p->unload();
                AssetPool<T>::empty.push_back(p.index);
                map.erase(name);
                names[p.index] = "";
            } else {
                Log::error("asset not found");
            }
        }
        void checkBegin() {
            AssetPool<T>::checkBegin();
        }
        void check(const Asset<T>& p) {
            AssetPool<T>::check(p);
        }
        void checkEnd(std::function<void(Asset<T>&)>&& garbageCallback) {
            AssetPool<T>::checkEnd(std::move(garbageCallback));
        }
    };

    template<typename T>
    bool Asset<T>::valid() const {
        return 0 < index && index < pool->array.size();
    }

    template<typename T>
    T& Asset<T>::get() {
        if (0 < index && index < pool->array.size()) {
            return pool->array[index];
        } else {
            Log::error("invalid pointer");
            return pool->array[index];
        }
    }

    template<typename T>
    const T& Asset<T>::get() const {
        if (0 < index && index < pool->array.size()) {
            return pool->array[index];
        } else {
            Log::error("invalid pointer");
            return pool->array[index];
        }
    }

    template<typename T>
    void Asset<T>::free() {
        pool->free(*this);
    }

}
