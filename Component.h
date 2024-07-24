#pragma once

#include <unordered_map>
#include <iostream>
#include <tuple>
#include <functional>
#include <optional>

#include "Object.h"
//#include "System.h"
// 
namespace cmp {
    struct Position {
        float x;
        float y;

        Position(float x, float y) :x{ x }, y{ y } {}
        Position(vec2<float> v) : Position{ v.x, v.y } {}
        operator vec2<float>() { return { x, y }; }
    };
    struct Velocity {
        float dx;
        float dy;
        Velocity(float dx, float dy) :dx{ dx }, dy{ dy } {}
        Velocity(vec2<float> v) : Velocity{ v.x, v.y } {}
        operator vec2<float>() { return { dx, dy }; }
    };
    struct Rotation {
        float phi;
    };
    struct Sprite {
        vec2<float> v1;
        vec2<float> v2;
        vec2<float> v3;
    };
};

template <typename ...Components>
struct Registry {

    using EntityId = uint64_t;

    template <typename T>
    using ComponentStorage = std::vector<std::pair<EntityId, T>>;

    template <typename T>
    using ComponentMap = std::unordered_map< EntityId, typename ComponentStorage<T>::iterator>; // this std::any is a pointer to specific iterator;

    using AllComponentMap = std::tuple<ComponentMap<Components>...>;
    AllComponentMap Map;

    using AllComponentStorage = std::tuple<ComponentStorage<Components>...>;
    AllComponentStorage Storage;


    std::vector<EntityId> EntityStorage;

    static EntityId getId() {
        static std::atomic<EntityId> Id = 0;
        return Id++;
    }

    EntityId create() {
        return EntityStorage.emplace_back(getId());
    }

    template <typename T>
    auto& getCmp() {
        return std::get<ComponentStorage<T>>(Storage);
    }
    template <typename T>
    auto& getMap() {
        return std::get<ComponentMap<T>>(Map);
    }

    template<typename T, typename ...Args>
    void emplace(EntityId Id, Args && ... args) {
        auto& CmpStorage = getCmp<T>();

        CmpStorage.emplace_back(std::pair{ Id, T{ std::forward<Args>(args)... } });

        auto It = std::prev(CmpStorage.end());

        auto& CmpMap = getMap<T>();
        auto [_, IsInserted] = CmpMap.emplace(Id, It);
        if (!IsInserted)
            throw std::runtime_error{ "Component for entiry has been already recorded\n" };
    }

    template<typename T>
    auto& view() {
        return getCmp<T>();
    }

    template<typename T>
    T* findComponentOrNull(EntityId Ent) {
        auto& CmpMap = getMap<T>();
        auto FindIt = CmpMap.find(Ent);
        if (FindIt == CmpMap.end())
            return nullptr;

        return &FindIt->second->second;
    }

};