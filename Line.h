//
// Created by Ed on 20/12/2024.
//

#ifndef LINE_H
#define LINE_H

#include "Renderable.h"


class Line final : public Renderable {
public:
    Line(SDL_Color color, int dx, int dy);
    ~Line() override = default;
    void render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) override;
    [[nodiscard]] int w() const override { return abs(m_dx); };
    [[nodiscard]] int h() const override { return abs(m_dy); };
    void set_dx(const int dx) { m_dx = dx; };
    void set_dy(const int dy) { m_dy = dy; };
private:
    SDL_Color m_color;
    int m_dx;
    int m_dy;
};



#endif //LINE_H
