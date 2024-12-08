//
// Created by Edward Torbett on 10/12/2023.
//

#include "Renderable.h"

SDL_Color blend(const SDL_Color& a, const SDL_Color& b, const double alpha) {
    const double beta = 1.0f - alpha;
    return {
    static_cast<uint8_t>((a.r * alpha) + (b.r * beta)),
    static_cast<uint8_t>((a.g * alpha) + (b.g * beta)),
    static_cast<uint8_t>((a.b * alpha) + (b.b * beta))
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