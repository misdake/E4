#pragma once

#include <map>
#include <vector>
#include <memory.h>
#include <functional>
#include <string>

#include "../components/Components.h"

namespace E4 {

    template<typename T>
    struct Line {
        static constexpr uint8_t LINE_SIZE_POT = 4;
        static constexpr uint32_t LINE_SIZE = 1u << LINE_SIZE_POT; //16
        static constexpr uint32_t LINE_MASK = LINE_SIZE - 1;
        std::vector<uint32_t> empty;
        std::vector<T*> lists;
        std::vector<bool> valid;
        uint32_t next;
        uint32_t max;

        Line() :
            empty{},
            lists{},
            next{0},
            max{0} {
            alloc(); //make initial growth and '0' element
        }

        ~Line() {
            for (T* p : lists) {
                delete[] p;
            }
        }

        uint32_t alloc() {
            uint32_t i = next;
            if (!empty.empty()) {
                i = empty.back();
                valid[i] = true;
                empty.pop_back();
            } else {
                if (next >= max) {
                    T* list = new T[LINE_SIZE];
                    lists.emplace_back(list);
                    memset(list, 0, sizeof(T[LINE_SIZE]));
                    max += LINE_SIZE;
                }
                valid.emplace_back(true);
                next++;
            }
            return i;
        }

        void foreach(const std::function<void(T&)>& func) {
            for (uint32_t i = 1; i < next; i++) {
                if (valid[i]) {
                    T& t = get(i);
                    func(t);
                }
            }
        }

        T& get(uint32_t index) {
            T* targetList = lists[index >> LINE_SIZE_POT];
            T& target = targetList[index & LINE_MASK];
            return target;
        }

        void free(uint32_t index) {
            if (index > 0) {
                empty.push_back(index);
                T& t = get(index);
                valid[index] = false;
                memset(&t, 0, sizeof(T));
            }
        }
    };

    template<typename T>
    struct ComponentMember {
    private:
        uint32_t e;
    protected:
        template<typename... TYPES>
        friend
        class EcsBase;

        uint32_t& getRef() { return e; }
        uint32_t getVal() { return e; }
        const uint32_t getVal() const { return e; }
        bool _has() const { return e > 0; }
    };

    template<typename... TYPES>
    struct EntityCore;

    template<typename FIRST, typename ...REST>
    struct EntityCore<FIRST, REST...> : ComponentMember<FIRST>, EntityCore<REST...> {
    public:
        template<class T>
        inline uint32_t& get() {
            return this->ComponentMember<T>::getRef();
        }
        template<class T>
        inline bool has() const {
            return this->ComponentMember<T>::_has();
        }
    };

    template<>
    struct EntityCore<> {
        uint32_t name; // index of app.stringPool
        uint64_t id;
        uint32_t index;
        uint32_t parent;
    };

    template<typename T>
    struct EcsComponent {
    private:
        Line<T> line;
    protected:
        template<typename... TYPES>
        friend
        class EcsBase;

        T& createComponent(uint32_t& index) {
            if (index > 0) return line.get(index);
            index = line.alloc();
            return line.get(index);
        }
        T& getComponent(uint32_t index) {
            return line.get(index);
        }
        void removeComponent(uint32_t& index) {
            line.free(index);
            index = 0;
        }
    };

    template<typename... TYPES>
    struct EcsBase;

    template<typename FIRST, typename ...REST>
    struct EcsBase<FIRST, REST...> : EcsComponent<FIRST>, EcsBase<REST...> {
    public:
        template<class T>
        T& create(ComponentMember<T>& entityCore) {
            return this->EcsComponent<T>::createComponent(entityCore.getRef());
        }
        template<class T>
        bool has(ComponentMember<T>& entityCore) {
            return entityCore._has();
        }
        template<class T>
        T& getOrCreate(ComponentMember<T>& entityCore) {
            if (!entityCore._has()) {
                return create<T>(entityCore);
            } else {
                return get<T>(entityCore);
            }
        }
        template<class T>
        T& getOrCreate(ComponentMember<T>& entityCore, bool& created) {
            if (!entityCore._has()) {
                created = true;
                return create<T>(entityCore);
            } else {
                created = false;
                return get<T>(entityCore);
            }
        }
        template<class T>
        T& get(ComponentMember<T>& entityCore) {
            return this->EcsComponent<T>::getComponent(entityCore.getVal());
        }
        template<class T>
        void fortype(const std::function<void(T&)>& func) {
            return this->EcsComponent<T>::line.foreach(func);
        }
        template<class T>
        void remove(ComponentMember<T>& entityCore) {
            this->EcsComponent<T>::removeComponent(entityCore.getRef());
        }
    };

    template<>
    struct EcsBase<> {
    };

    template<typename... TYPES>
    struct EcsCore : public EcsBase<TYPES...> {
    private:
        uint64_t nextEntityId = 1;
        std::map<uint64_t, uint32_t> entityIds; //id => entities
        Line<EntityCore<TYPES...>> entities;

    public:
        EntityCore<TYPES...>& newEntity() {
            uint32_t index = entities.alloc();
            EntityCore<TYPES...>& entity = entities.get(index);
            entity.id = nextEntityId;
            entity.index = index;
            entityIds.insert(std::make_pair(entity.id, entity.index));
            nextEntityId++;
            return entity;
        }
        EntityCore<TYPES...>& getEntityById(uint64_t id) {
            return entities.get(entityIds[id]);
        }
        EntityCore<TYPES...>& getEntityByIndex(uint32_t index) {
            return entities.get(index);
        }
        void deleteEntity(EntityCore<TYPES...>& entity) {
            (this->EcsBase<TYPES...>::template remove<TYPES>(entity), ...);
            entityIds.erase(entity.id);
            entities.free(entity.index);
        }

        void clear() {
            for (auto[id, index] : entityIds) {
                EntityCore<TYPES...>& entity = entities.get(index);
                (this->EcsBase<TYPES...>::template remove<TYPES>(entity), ...);
                entities.free(entity.index);
            }
        }
        void foreach(std::function<void(EntityCore<TYPES...>&)>&& func) {
            for (auto[id, index] : entityIds) {
                EntityCore<TYPES...>& entity = entities.get(index);
                func(entity);
            }
        }
        template<typename ...PARAMS, typename FUNC>
        void fortypes(FUNC&& func) {
            for (auto[id, index] : entityIds) { //std::map supports insertion (no restriction), and deletion (not the current one) while iterating
                auto& entity = entities.get(index);
                if ((entity.template has<PARAMS>() && ...)) {
                    func(entity, (this->EcsBase<TYPES...>::template get<PARAMS>(entity))...);
                }
            }
        }
    };

    using Ecs = EcsCore<Transform, Drawable, Script, Env>;
    using Entity = EntityCore<Transform, Drawable, Script, Env>;

}