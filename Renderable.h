//
// Created by Edward Torbett on 10/12/2023.
//

#ifndef MAGICMIRROR_RENDERABLE_H
#define MAGICMIRROR_RENDERABLE_H

#include <memory>
#include <vector>
#include <chrono>
#include <SDL.h>

const SDL_Color BLACK  = {0x00, 0x00, 0x00, 0xFF };
const SDL_Color WHITE  = {0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Color GREY  = {0x80, 0x80, 0x80, 0xFF };
const SDL_Color GREEN  = {0x80, 0xFF, 0x80, 0xFF };
const SDL_Color BLUE   = {0x80, 0x80, 0xFF, 0xFF };
const SDL_Color YELLOW = {0xFF, 0xFF, 0x00, 0xFF };
const SDL_Color RED    = {0xFF, 0x00, 0x00, 0xFF };
const SDL_Color AMBER  = {0xFF, 0xA0, 0x00, 0xFF };

float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &start, const std::chrono::time_point<std::chrono::steady_clock> &end);
float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &s1, const std::chrono::time_point<std::chrono::steady_clock> &e1, const std::chrono::time_point<std::chrono::steady_clock> &s2, const std::chrono::time_point<std::chrono::steady_clock> &e2);

SDL_Color blend(const SDL_Color& a, const SDL_Color& b, double alpha);

class Renderable {
public:
    Renderable();
    virtual ~Renderable();
    virtual void render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now);
    [[nodiscard]] int x() const { return m_x; };
    [[nodiscard]] int y() const { return m_y; };
    [[nodiscard]] virtual int w() const = 0;
    [[nodiscard]] virtual int h() const = 0;
    void set_transient(const bool transient) { m_transient = transient; };
    void set_pos(int x, int y);
    void set_visible(bool visible, const std::chrono::time_point<std::chrono::steady_clock> &now);
    [[nodiscard]] bool visible() const { return m_visible; }
private:
    int m_x;
    int m_y;
    std::vector<std::shared_ptr<Renderable>> m_children;
    bool m_visible;
    bool m_transient{};
    std::chrono::time_point<std::chrono::steady_clock> m_lastVisibleChangeTime;
protected:
    void add_child(const std::shared_ptr<Renderable>& child, int x, int y);
    void clear_children();
    [[nodiscard]] bool skip_render(const std::chrono::time_point<std::chrono::steady_clock> &now) const;
    [[nodiscard]] float get_brightness(const std::chrono::time_point<std::chrono::steady_clock> &now) const;
};


#endif //MAGICMIRROR_RENDERABLE_H
