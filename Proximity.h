//
// Created by Ed on 02/01/2024.
//

#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <chrono>
#include <connection.h>
#include <string>

#include "Renderable.h"
#include "RenderableText.h"


class Proximity : Renderable {
public:
    explicit Proximity(const std::string &symbol);
    ~Proximity() override;
    void set_entity(const std::string& entity);
    void fetch(RestClient::Connection *connection);
    void render(SDL_Renderer* renderer) override;
    void set_pos(int x, int y) override;
    [[nodiscard]] bool is_interesting() const;
    [[nodiscard]] int h() const override;
    [[nodiscard]] int w() const override;
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
private:
    std::string m_entity;
    bool m_travelling;
    int m_proximity;
    std::chrono::time_point<std::chrono::steady_clock> m_start_time;
    std::chrono::time_point<std::chrono::steady_clock> m_poll_time;
    int m_start_proximity;
    double m_speed;
    int m_x;
    int m_y;

    RenderableText *m_startSymbol;
    RenderableText *m_endSymbol;
    RenderableText *m_travellerSymbol;
    RenderableText *m_eta;
};



#endif //PROXIMITY_H
