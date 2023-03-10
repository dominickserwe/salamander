#include "platform.hpp"

#include <SDL.h>
#include <iostream>

void Input::clear() {
    m_keys_pressed.fill(false);
    m_keys_released.fill(false);
}

void Input::set_mouse_pos(float x, float y) {
    m_mouse_x = x;
    m_mouse_y = y;
}

void Input::set_key(int key, bool is_down, bool was_down) {
    if (key < 0 || key >= _BUFFER_SIZE) {
        return;
    }

    m_keys_down[key] = is_down;
    m_keys_pressed[key] = is_down && !was_down;
    m_keys_released[key] = !is_down && was_down;
}

bool Input::is_key_down(int key) const {
    if (key < 0 || key >= _BUFFER_SIZE) {
        return false;
    }

    return m_keys_down[key];
}

bool Input::is_key_pressed(int key) const {
    if (key < 0 || key >= _BUFFER_SIZE) {
        return false;
    }

    return m_keys_pressed[key];
}

bool Input::is_key_released(int key) const {
    if (key < 0 || key >= _BUFFER_SIZE) {
        return false;
    }

    return m_keys_released[key];
}

Platform::Platform(const std::string &title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "[ERROR]: SDL failed to init" << std::endl;
        exit(-1);
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cout << "[ERROR]: SDL failed to create a window" << std::endl;
        exit(-1);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cout << "[ERROR]: SDL failed to create a renderer" << std::endl;
        exit(-1);
    }

    m_running = true;
}

Platform::~Platform() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Platform::handle_events(Input &input) {
    SDL_Event event;

    input.clear();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                m_running = false;
                break;
            }

            case SDL_MOUSEMOTION: {
                input.set_mouse_pos((float)event.motion.x, (float)event.motion.y);
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                auto key = event.key.keysym.sym;

                bool is_down = event.key.state == SDL_PRESSED;
                bool was_down = (event.key.state == SDL_RELEASED) || (event.key.repeat != 0);

                input.set_key(key, is_down, was_down);

                break;
            }
        }
    }

}