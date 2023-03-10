#pragma once

#include "renderer.hpp"

#include <SDL.h>
#include <vector>

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

    void toggle_solid_tile(int tx, int ty);
    void draw_grid(Renderer &renderer, float depth);
    
    bool is_tile_solid(int tx, int ty) const;
    int get_tile_size() const { return m_tile_size; }
};