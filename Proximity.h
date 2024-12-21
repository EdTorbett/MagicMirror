//
// Created by Ed on 02/01/2024.
//

#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <chrono>
#include <connection.h>
#include <string>

#include "Line.h"
#include "Renderable.h"
#include "RenderableText.h"


class Proximity final : public Renderable {
public:
    explicit Proximity(const std::string &symbol);
    ~Proximity() override;
    void set_entity(const std::string& entity);
    void fetch(RestClient::Connection *connection);
    void render(SDL_Renderer* renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) override;
    [[nodiscard]] bool is_interesting() const;
    [[nodiscard]] int h() const override;
    [[nodiscard]] int w() const override;
private:
    std::string m_entity;
    bool m_travelling;
    int m_proximity;
    std::chrono::time_point<std::chrono::steady_clock> m_start_time;
    std::chrono::time_point<std::chrono::steady_clock> m_poll_time;
    int m_start_proximity;
    double m_speed;

    std::shared_ptr<RenderableText> m_startSymbol;
    std::shared_ptr<RenderableText> m_endSymbol;
    std::shared_ptr<RenderableText> m_travellerSymbol;
    std::shared_ptr<RenderableText> m_eta;
    std::shared_ptr<Line> m_line1;
    std::shared_ptr<Line> m_line2;
};



#endif //PROXIMITY_H
