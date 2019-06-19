#pragma once

#include <map>
#include <vector>
#include <memory.h>
#include <functional>

#include "../components/Components.h"

namespace E4 {

    template<typename T>
    struct Line {
        static constexpr uint8_t LINE_SIZE_POT = 4;
        static constexpr uint32_t LINE_SIZE = 1u << LINE_SIZE_POT;
        static constexpr uint32_t LINE_MASK = LINE_SIZE - 1;
        std::vector<uint32_t> empty;
        std::vector<T*> lists;
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
                empty.pop_back();
            } else {
                if (next >= max) {
                    T* list = new T[LINE_SIZE];
                    lists.emplace_back(list);
                    memset(list, 0, sizeof(T[LINE_SIZE]));
                    max += LINE_SIZE;
                }
                next++;
            }
            return i;
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
        uint64_t id;
        uint32_t index;
    };

    template<typename T>
    struct EcsComponent {
    private:
        Line<T> line;
    protected:
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
        T& get(ComponentMember<T>& entityCore) {
            return this->EcsComponent<T>::getComponent(entityCore.getVal());
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

        void foreach(std::function<void(EntityCore<TYPES...>&)>&& func) {
            for (auto[id, index] : entityIds) {
                EntityCore<TYPES...>& entity = entities.get(index);
                func(entity);
            }
        }
        template<typename ...PARAMS, typename FUNC>
        void fortype(FUNC&& func) {
            for (auto[id, index] : entityIds) {
                auto& entity = entities.get(index);
                if ((entity.template has<PARAMS>() && ...)) {
                    func(entity, (this->EcsBase<TYPES...>::template get<PARAMS>(entity))...);
                }
            }
        }
    };

    using Ecs = EcsCore<Transform, Drawable, Script>;
    using Entity = EntityCore<Transform, Drawable, Script>;

}