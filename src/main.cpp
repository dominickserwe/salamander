#include "platform.hpp"
#include "renderer.hpp"

#include "world.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <SDL.h>

struct Entity {
    int id;

    float x;
    float y;

    GridBody body;
};

int main(int argc, char **argv) {

    Input input;

    Platform platform("SAL", 1280, 720);
    Renderer renderer(platform.get_renderer());

    World world(1280, 720, 32);

    Entity player = {};
    player.body.x = 2;
    player.body.y = 2;

    world.add_body(&player.body);

    int counter = 0;

    while (platform.is_running()) {
        platform.handle_events(input);

        if (input.is_key_down(' ')) {
            auto mx = input.get_mouse_x();
            auto my = input.get_mouse_y();

            world.toggle_solid_tile(mx / world.get_tile_size(), my / world.get_tile_size());
        }

        player.body.vx = input.is_key_down('d') - input.is_key_down('a');
        player.body.vy = input.is_key_down('s') - input.is_key_down('w');

        world.step(60);

        player.x = player.body.x * world.get_tile_size();
        player.y = player.body.y * world.get_tile_size();

        world.draw_grid(renderer, 0);
        renderer.push_rectangle({ player.x, player.y, (float)world.get_tile_size(), (float)world.get_tile_size() }, { 96, 0, 255, 255 }, 0);

        renderer.present();
    }

    return 0;
}