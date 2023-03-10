#pragma once

#include <SDL.h>
#include <vector>

enum RenderType {
    NONE,
    LINE,
    RECT,
    TEXTURE,
};

struct RenderData {
    RenderType type;

    void *data;
    float depth;
};

class Renderer {
private:
    SDL_Renderer *m_renderer = NULL;
    std::vector<RenderData> m_data = {};

public:
    Renderer(SDL_Renderer *renderer);

    void push_line(float x1, float y1, float x2, float y2, SDL_Color color, float depth);
    void push_rectangle(SDL_FRect destination, SDL_Color color, float depth);
    void push_texture(SDL_Texture *texture, SDL_FRect destination, SDL_Color color, float depth);

    void present();
};