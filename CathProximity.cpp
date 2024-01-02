//
// Created by Ed on 02/01/2024.
//

#include "CathProximity.h"

CathProximity::CathProximity(): m_proximity(0), m_start_time(0), m_start_proximity(0), m_x(0), m_y(0) {

}

CathProximity::~CathProximity() = default;


void CathProximity::set_entity(const std::string& entity) {
    m_entity = entity;
}

void CathProximity::fetch(RestClient::Connection* connection) {
    //do nothing
}

void CathProximity::set_pos(int x, int y) {
    m_x = x;
    m_y = y;
}

int CathProximity::x() const {
    return m_x;
}

int CathProximity::y() const {
    return m_y;
}

int CathProximity::w() const {
    return 600;
}

int CathProximity::h() const {
    return 600;
}

void CathProximity::render(SDL_Renderer* renderer) {
    //do nothing
}
