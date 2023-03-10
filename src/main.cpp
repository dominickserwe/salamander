#include "platform.hpp"
#include "renderer.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <SDL.h>

class World {
private:
    int m_width = 0;
    int m_height = 0;
    int m_tile_size = 0;

    //TODO: probably faster and easier with an array
    std::vector<SDL_Point> m_solids;

public:
    World(int width, int height, int tile_size) {
        m_width = width;
        m_height = height;
        m_tile_size = tile_size;
    }

    void toggle_solid_tile(int tx, int ty) {
        if ((tx * m_tile_size) < 0 || (tx * m_tile_size >= m_width) || (ty * m_tile_size) < 0 || (ty * m_tile_size) >= m_height) {
            return;
        }

        auto itr = std::find_if(m_solids.begin(), m_solids.end(), [tx, ty](const SDL_Point &current) -> bool {
            return (current.x == tx) && (current.y == ty);
        });

        if (itr != m_solids.end()) {
            //m_solids.erase(itr);
            return;
        }

        m_solids.push_back({ tx, ty });
    }

    void draw_grid(Renderer &renderer, float depth) {
        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Color red = { 255, 0, 0, 255 };

        for (auto solid : m_solids) {
            float x = solid.x * m_tile_size;
            float y = solid.y * m_tile_size;

            renderer.push_rectangle({ x, y, (float)m_tile_size, (float)m_tile_size }, red, depth);
        }

        for (int y = 0; y < m_height; y += m_tile_size) {
            renderer.push_line(0, y, m_width, y, white, depth);
        }

        for (int x = 0; x < m_width; x += m_tile_size) {
            renderer.push_line(x, 0, x, m_height, white, depth);
        }
    }

    bool is_tile_solid(int tx, int ty) const {
        auto itr = std::find_if(m_solids.begin(), m_solids.end(), [tx, ty](const SDL_Point &current) -> bool {
            return (current.x == tx) && (current.y == ty);
        });

        return itr != m_solids.end();
    }

    int get_tile_size() const { return m_tile_size; }
};

int main(int argc, char **argv) {

    Input input;
    Platform platform("SAL", 1280, 720);
    Renderer renderer(platform.get_renderer());

    World world(1280, 720, 32);

    float vx = 0;
    float vy = 0;

    float x = 2;
    float y = 2;

    int counter = 0;

    while (platform.is_running()) {
        platform.handle_events(input);

        if (input.is_key_down(' ')) {
            auto mx = input.get_mouse_x();
            auto my = input.get_mouse_y();

            world.toggle_solid_tile(mx / world.get_tile_size(), my / world.get_tile_size());
        }

        vx = input.is_key_down('d') - input.is_key_down('a');
        vy = input.is_key_down('s') - input.is_key_down('w');

        if (++counter >= 60) {
            if (world.is_tile_solid(x + vx, y)) {
                vx = 0;
            }

            if (world.is_tile_solid(x, y + vy)) {
                vy = 0;
            }

            x += vx;
            y += vy;

            counter = 0;
        }

        world.draw_grid(renderer, 0);
        renderer.push_rectangle({ x * world.get_tile_size(), y * world.get_tile_size(), (float)world.get_tile_size(), (float)world.get_tile_size() }, { 255, 0, 255, 255 }, 0);

        renderer.present();
    }

    return 0;
}