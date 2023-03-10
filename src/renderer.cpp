#include "renderer.hpp"

#include <algorithm>

struct LineData {
    float x1;
    float y1;

    float x2;
    float y2;

    SDL_Color color;  
};

struct RectData {
    SDL_FRect rect;
    SDL_Color color;
};

struct TextureData {
    SDL_Texture *texture;
    SDL_FRect destination;
    SDL_Color color;
};

Renderer::Renderer(SDL_Renderer *renderer) {
    m_renderer = renderer;
}

void Renderer::push_line(float x1, float y1, float x2, float y2, SDL_Color color, float depth) {
    auto line_data = new LineData;

    line_data->x1 = x1;
    line_data->y1 = y1;

    line_data->x2 = x2;
    line_data->y2 = y2;

    line_data->color = color;

    m_data.push_back({ RenderType::LINE, (void *)line_data, depth });
}

void Renderer::push_rectangle(SDL_FRect destination, SDL_Color color, float depth) {
    auto rect_data = new RectData;

    rect_data->rect = destination;
    rect_data->color = color;

    m_data.push_back({ RenderType::RECT, (void *)rect_data, depth });
}

void Renderer::push_texture(SDL_Texture *texture, SDL_FRect destination, SDL_Color color, float depth) {
    //TODO: currently drawing pink quad if no texture is given, SDL defaults to not drawing anything
    if (!texture) {
        push_rectangle(destination, { 255, 0, 255, color.a }, depth);
        return;
    }

    auto texture_data = new TextureData;

    texture_data->texture = texture;
    texture_data->destination = destination;
    texture_data->color = color;

    m_data.push_back({ RenderType::TEXTURE, (void *)texture_data, depth });
}

void Renderer::present() {
    std::sort(m_data.begin(), m_data.end(), [](const RenderData &a, const RenderData &b) -> bool {
        return a.depth > b.depth;
    });

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    for (auto &data : m_data) {
        switch (data.type) {
            case RenderType::LINE: {
                auto line_data = (LineData *)data.data;

                SDL_SetRenderDrawColor(m_renderer, line_data->color.r, line_data->color.g, line_data->color.b, line_data->color.a);
                SDL_RenderDrawLineF(m_renderer, line_data->x1, line_data->y1, line_data->x2, line_data->y2);
            
                break;
            }

            case RenderType::RECT: {
                auto rect_data = (RectData *)data.data;

                SDL_SetRenderDrawColor(m_renderer, rect_data->color.r, rect_data->color.g, rect_data->color.b, rect_data->color.a);
                SDL_RenderFillRectF(m_renderer, &rect_data->rect);

                break;
            }

            case RenderType::TEXTURE: {
                auto texture_data = (TextureData *)data.data;

                if (!texture_data->texture) break;

                SDL_SetTextureAlphaMod(texture_data->texture, texture_data->color.a);
                SDL_SetTextureColorMod(texture_data->texture, texture_data->color.r, texture_data->color.g, texture_data->color.b);
                SDL_RenderCopyF(m_renderer, texture_data->texture, NULL, &texture_data->destination);

                break;
            }
        }

        delete data.data;
    }

    SDL_RenderPresent(m_renderer);
    m_data.clear();
}