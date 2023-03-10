#pragma once

#include <string>
#include <array>

struct SDL_Window;
struct SDL_Renderer;

class Input {
private:
    static constexpr int _BUFFER_SIZE = 256;

    float m_mouse_x = 0.0f;
    float m_mouse_y = 0.0f;

    std::array<bool, _BUFFER_SIZE> m_keys_down = {};
    std::array<bool, _BUFFER_SIZE> m_keys_pressed = {};
    std::array<bool, _BUFFER_SIZE> m_keys_released = {};

public:
    void clear();

    void set_mouse_pos(float x, float y);
    void set_key(int key, bool is_down, bool was_down);

    float get_mouse_x() const { return m_mouse_x; }
    float get_mouse_y() const { return m_mouse_y; }

    bool is_key_down(int key) const;
    bool is_key_pressed(int key) const;
    bool is_key_released(int key) const;
};

class Platform {
private:
    SDL_Window *m_window = NULL;
    SDL_Renderer *m_renderer = NULL;
    bool m_running = false;

public:
    Platform(const std::string &title, int width, int height);
    ~Platform();

    void handle_events(Input &input);

    SDL_Window *get_window() const { return m_window; }
    SDL_Renderer *get_renderer() const { return m_renderer; }

    bool is_running() const { return m_running; }
};