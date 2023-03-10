#include "platform.hpp"
#include "renderer.hpp"

#include "world.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <SDL.h>

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
        renderer.push_rectangle({ x * world.get_tile_size(), y * world.get_tile_size(), (float)world.get_tile_size(), (float)world.get_tile_size() }, { 96, 0, 255, 255 }, 0);

        renderer.present();
    }

    return 0;
}