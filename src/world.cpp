#include "world.hpp"

#include <algorithm>

World::World(int width, int height, int tile_size) {
    m_width = width;
    m_height = height;
    m_tile_size = tile_size;
}

void World::step(int speed) {
    if (++m_counter < speed) return;

    for (auto body : m_bodies) {
        if (is_tile_solid(body->x + body->vx, body->y)) {
            body->vx = 0;
        }

        if (is_tile_solid(body->x, body->y + body->vy)) {
            body->vy = 0;
        }

        body->x += body->vx;
        body->y += body->vy;
    }

    m_counter = 0;
}

void World::toggle_solid_tile(int tx, int ty) {
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

void World::draw_grid(Renderer &renderer, float depth) {
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

bool World::is_tile_solid(int tx, int ty) const {
    auto itr = std::find_if(m_solids.begin(), m_solids.end(), [tx, ty](const SDL_Point &current) -> bool {
        return (current.x == tx) && (current.y == ty);
    });

    return itr != m_solids.end();
}