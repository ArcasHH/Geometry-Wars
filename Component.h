#pragma once

#include <unordered_map>
#include <iostream>
#include <tuple>
#include <functional>
#include <optional>
#include <cassert>

#include "Math.h"

using EntityId = uint64_t;


namespace cmp {

    struct TriangleSprite { // Triangle : three points formed by vectors from the point(0,0)
        Triangle sprite;
    };
    union Color {
        struct {
            unsigned char b;
            unsigned char g;
            unsigned char r;
            unsigned char a;
        };
        uint32_t data;
        Color() : data(0x000000ff) {}
        Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : r{ R }, g{ G }, b{ B }, a{ A } { }
        Color& operator=(const Color&) = default;
        Color& operator+=(const Color& other) {
            b += other.b;
            g += other.g;
            r += other.r;
            return *this;
        }
    };

    //Transform components:
    struct Position {
        vec2<float> position; // r-vector from (0,0)
    };
    struct Velocity {
        vec2<float> velocity;
        float max_speed;
        float speed_scale;
    };
    struct Rotation {
        float phi;
    };
    
    struct Direction { // the direction the object is looking at. If it can
        vec2<float> direction;
    };

    //defines the type of the object
    struct IsPlayer {
        int curr_score;
    };
    struct IsEnemy {
        EntityId player_id;
        int cost; // score += cost while kill enemy
    };
    struct IsBullet {};


    struct IsActive { // object can act (not static)
        bool is_active;
        
    };

    struct Health {
        int curr_health;
        int max_health;
        
        bool can_regenerate;
        float curr_regeneration_time;
        float regeneration_time;
    };
    struct Damage {
        int damage;
    };

    struct Collision {
        float collide_distance;
    };


    // for shooting:
    struct CanShoot {
        bool can_shoot; 
        float reload_timer;
        bool is_shoot{ false };
    };
    struct Ammo {
        using AmmoStorage = std::vector<EntityId>;
        AmmoStorage ammo_store;
        Ammo(AmmoStorage &&Storage ): ammo_store{std::move(Storage)}{}
    };

    //Scenario
    struct Enemies {
        using EnemyStorage = std::vector<EntityId>;
        EnemyStorage enemy_store;
        Enemies(EnemyStorage&& Storage) : enemy_store{ std::move(Storage) } {}
    };
    struct Progress {
        int score;
        float curr_time;
        float timer_before_act{ 1.f };
    };

};





template <typename ...Components>
struct Registry {

    template <typename T>
    struct IdxWrapper final {
        size_t Val;
        IdxWrapper() = default;
        IdxWrapper(size_t Val) : Val{Val} {}
        operator size_t() const { return Val;  }
    };

    template <typename T>
    using ComponentStorage = std::vector<std::pair<EntityId, T>>;

    template <typename T>
    using ComponentMap = std::unordered_map< EntityId, IdxWrapper<T>>;

    using AllComponentMap = std::tuple<ComponentMap<Components>...>;
    AllComponentMap Map;

    using AllComponentStorage = std::tuple<ComponentStorage<Components>...>;
    AllComponentStorage Storage;

    template <typename T, typename Tuple>
    struct has_type;
    template <typename T, typename... Us>
    struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<ComponentStorage<T>, Us>...> {};

    std::vector<EntityId> EntityStorage;

    static EntityId getId() {
        static EntityId Id = 0;
        return Id++;
    }

    EntityId create() {
        return EntityStorage.emplace_back(getId());
    }

    template <typename T>
    auto& getCmp() {
        static_assert(has_type<T, AllComponentStorage>(), "Accessing commonent was not recorded.");
        return std::get<ComponentStorage<T>>(Storage);
    }
    template <typename T>
    auto& getMap() {
        static_assert(has_type<T, AllComponentStorage>(), "Accessing commonent was not recorded.");
        return std::get<ComponentMap<T>>(Map);
    }

    template<typename T, typename ...Args>
    void emplace(EntityId Id, Args && ... args) {
        auto& CmpStorage = getCmp<T>();

        auto Size = CmpStorage.size();
        CmpStorage.emplace_back(std::pair{ Id, T{ std::forward<Args>(args)... } });

        auto& CmpMap = getMap<T>();
        auto [_, IsInserted] = CmpMap.emplace(Id, Size);
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

        auto Idx = FindIt->second;
        return &getCmp<T>()[Idx].second;
    }

    template<typename T>
    T* findComponent(EntityId Ent) {
        auto* Ptr = findComponentOrNull<T>(Ent);
        assert(Ptr && "Accessing not register component");
        return Ptr;
    }

    EntityId getPlayer() {
        auto& PlayerView = view<cmp::IsPlayer>();
        auto&& [PlayerEnt, _] = PlayerView.front();
        return PlayerEnt;
    }

};


