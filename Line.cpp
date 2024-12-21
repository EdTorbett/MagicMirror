//
// Created by Ed on 20/12/2024.
//

#include "Line.h"


Line::Line(const SDL_Color color, const int dx, const int dy) :
    m_color(color),
    m_dx(dx),
    m_dy(dy) {
    //pass
}

void Line::render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) {
    if (skip_render(now)) {
        return;
    }
    SDL_Color prev_color;
    SDL_GetRenderDrawColor(renderer, &prev_color.r, &prev_color.g, &prev_color.b, &prev_color.a);
    auto [r, g, b, a] = blend(m_color, BLACK, get_brightness(now));
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, this->y(), 1080 - this->x(), this->y() + m_dy, 1080 - (this->x() + m_dx));
    SDL_SetRenderDrawColor(renderer, prev_color.r, prev_color.g, prev_color.b, prev_color.a);
}