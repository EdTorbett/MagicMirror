//
// Created by Ed on 02/01/2024.
//

#ifndef CATHPROXIMITY_H
#define CATHPROXIMITY_H
#include <connection.h>
#include <string>

#include "Renderable.h"


class CathProximity : Renderable {
public:
    CathProximity();
    ~CathProximity();
    void set_entity(const std::string& entity);
    void fetch(RestClient::Connection *connection);
    void render(SDL_Renderer* renderer) override;
    void set_pos(int x, int y) override;
    [[nodiscard]] int h() const override;
    [[nodiscard]] int w() const override;
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
private:
    std::string m_entity;
    int m_proximity;
    time_t m_start_time;
    int m_start_proximity;
    int m_x;
    int m_y;
};



#endif //CATHPROXIMITY_H
