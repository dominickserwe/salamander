#pragma once

#include "renderer.hpp"

#include <SDL.h>
#include <vector>

struct GridBody {
    float x;
    float y;

    float vx;
    float vy;
};

class World {
private:
    int m_width = 0;
    int m_height = 0;
    int m_tile_size = 0;

    int m_counter = 0;

    //TODO: probably faster and easier with an array
    std::vector<SDL_Point> m_solids;
    std::vector<GridBody *> m_bodies;

public:
    World(int width, int height, int tile_size);

    void step(int speed);

    void toggle_solid_tile(int tx, int ty);
    void draw_grid(Renderer &renderer, float depth);

    void add_body(GridBody *body) {
        m_bodies.push_back(body);
    }

    void remove_body(GridBody *body) {
        auto itr = std::find(m_bodies.begin(), m_bodies.end(), body);
        if (itr != m_bodies.end()) {
            m_bodies.erase(itr);
        }
    }

    bool is_tile_solid(int tx, int ty) const;
    int get_tile_size() const { return m_tile_size; }
};