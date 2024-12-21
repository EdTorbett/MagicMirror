//
// Created by Edward Torbett on 10/12/2023.
//

#include "Renderable.h"

SDL_Color blend(const SDL_Color& a, const SDL_Color& b, const double alpha) {
    const double beta = 1.0f - alpha;
    return {
    static_cast<uint8_t>((a.r * alpha) + (b.r * beta)),
    static_cast<uint8_t>((a.g * alpha) + (b.g * beta)),
    static_cast<uint8_t>((a.b * alpha) + (b.b * beta)),
    static_cast<uint8_t>((a.a * alpha) + (b.a * beta))
    };
}

float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &start, const std::chrono::time_point<std::chrono::steady_clock> &end) {
    if (now < start) {
        return 0.0f;
    }
    if (now > end) {
        return 1.0f;
    }
    const auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    const auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return static_cast<float>(d1.count()) / static_cast<float>(d2.count());
}

float time_ramp(const std::chrono::time_point<std::chrono::steady_clock> &now, const std::chrono::time_point<std::chrono::steady_clock> &s1, const std::chrono::time_point<std::chrono::steady_clock> &e1, const std::chrono::time_point<std::chrono::steady_clock> &s2, const std::chrono::time_point<std::chrono::steady_clock> &e2) {
    if (now < s2) {
        return time_ramp(now, s1, e1);
    }
    return 1.0f - time_ramp(now, s2, e2);
}

Renderable::Renderable():
m_x(0),
m_y(0),
m_visible(false),
m_children()
{
    //pass
}

Renderable::~Renderable() {
    this->m_children.clear();
}

void Renderable::render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) { //NOLINT
    if (skip_render(now)) {
        return;
    }
    for (const auto &child : m_children) {
        child->render(renderer, now);
    }
}

void Renderable::set_visible(const bool visible, const std::chrono::time_point<std::chrono::steady_clock> &now) {
    if (m_visible != visible) {
        m_visible = visible;
        m_lastVisibleChangeTime = now;
        for (const auto &child : m_children) {
            child->set_visible(visible, now);
        }
    }
}

float Renderable::get_brightness(const std::chrono::time_point<std::chrono::steady_clock> &now) const {
    float brightness = 0.0f;
    if (m_transient) {
        if (m_visible) {
            brightness = time_ramp(now, m_lastVisibleChangeTime, m_lastVisibleChangeTime + std::chrono::seconds(2), m_lastVisibleChangeTime + std::chrono::seconds(8), m_lastVisibleChangeTime + std::chrono::seconds(10));
        } else {
            brightness = 1 - time_ramp(now, m_lastVisibleChangeTime, m_lastVisibleChangeTime + std::chrono::seconds(2));
        }
    } else {
        brightness = time_ramp(now, m_lastVisibleChangeTime, m_lastVisibleChangeTime + std::chrono::seconds(2));
        if (!m_visible) {
            brightness = 1 - brightness;
        }
    }
    return brightness;
}

bool Renderable::skip_render(const std::chrono::time_point<std::chrono::steady_clock> &now) const {
    return !m_visible && (now > m_lastVisibleChangeTime + std::chrono::seconds(2));
}

void Renderable::set_pos(const int x, const int y) {
    const int x_diff = x - m_x;
    const int y_diff = y - m_y;
    m_x = x;
    m_y = y;
    for (const auto &child : m_children) {
        child->set_pos(child->x() + x_diff, child->y() + y_diff);
    }
}

void Renderable::add_child(const std::shared_ptr<Renderable>& child, const int x, const int y) {
    child->set_pos(m_x + x, m_y + y);
    m_children.push_back(child);
}

void Renderable::clear_children() {
    m_children.clear();
}
