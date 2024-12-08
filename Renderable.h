//
// Created by Edward Torbett on 10/12/2023.
//

#ifndef MAGICMIRROR_RENDERABLE_H
#define MAGICMIRROR_RENDERABLE_H

#include <chrono>
#include <SDL.h>

const SDL_Color BLACK  = {0x00, 0x00, 0x00, 0 };
const SDL_Color WHITE  = {0xFF, 0xFF, 0xFF, 0 };
const SDL_Color GREEN  = {0x80, 0xFF, 0x80, 0 };
const SDL_Color BLUE   = {0x80, 0x80, 0xFF, 0 };
const SDL_Color YELLOW = {0xFF, 0xFF, 0x00, 0 };
const SDL_Color RED    = {0xFF, 0x00, 0x00, 0 };
const SDL_Color AMBER  = {0xFF, 0xA0, 0x00, 0 };

float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &start, const std::chrono::time_point<std::chrono::steady_clock> &end);
float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &s1, const std::chrono::time_point<std::chrono::steady_clock> &e1, const std::chrono::time_point<std::chrono::steady_clock> &s2, const std::chrono::time_point<std::chrono::steady_clock> &e2);

SDL_Color blend(const SDL_Color& a, const SDL_Color& b, double alpha);

class Renderable {
public:
    virtual ~Renderable() = default;
    virtual void render(SDL_Renderer *renderer) = 0;
    [[nodiscard]] virtual int x() const = 0;
    [[nodiscard]] virtual int y() const = 0;
    [[nodiscard]] virtual int w() const = 0;
    [[nodiscard]] virtual int h() const = 0;
    virtual void set_pos(int x, int y) = 0;
};


#endif //MAGICMIRROR_RENDERABLE_H
