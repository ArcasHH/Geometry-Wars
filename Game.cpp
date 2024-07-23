#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <unordered_map>
#include <iostream>
#include <tuple>

#include "Scene.h"
//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()


namespace cmp {
    struct Position {
        float x;
        float y;
    };
    // TODO
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
            std::cerr << "Component for entity " << Id << " has been already recorded\n";
    }

    template<typename T>
    auto& view() {
        return getCmp<T>();
    }

};

Registry<Triangle> Reg;

namespace sys {

    void draw(BuffTy Buffer) {

        auto& View = Reg.view<Triangle>();

        for (auto&& [Ent, Cmp] : View) {
            Cmp.draw(Buffer);
        }
    }


};

GameScene scene;
// initialize game data in this function
void initialize()
{
    scene.setPlayer(Triangle(vec2<float>(15, 45), vec2<float>(0, 0), vec2<float>(30, 0),  Color(255, 255, 255)), vec2<float>(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

    Triangle enemy_type1(Triangle(vec2<float>(15, 45), vec2<float>(0, 0), vec2<float>(30, 0), Color(200, 0, 0)));
    scene.addEnemy(enemy_type1, vec2<float>(100,100));
    scene.addEnemy(enemy_type1, vec2<float>(700, 700));
    scene.addEnemy(enemy_type1, vec2<float>(700, 500));


    auto FirstTriangle = Reg.create();

    Reg.emplace<Triangle>(FirstTriangle, vec2<float>(15, 45), vec2<float>(0, 0), vec2<float>(30, 0), Color(200, 200, 0));

    Reg.emplace<Triangle>(FirstTriangle, vec2<float>(15, 45), vec2<float>(100, 0), vec2<float>(30, 100), Color(200, 200, 200));
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  scene.act(dt);


}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  scene.draw(buffer);

  sys::draw(buffer);

}

// free game data in this function
void finalize()
{
}

