//
// Created by Ed on 02/01/2024.
//

#include "EdProximity.h"

EdProximity::EdProximity(): m_proximity(0), m_start_time(0), m_start_proximity(0), m_x(0), m_y(0) {
}

EdProximity::~EdProximity() = default;


void EdProximity::set_entity(const std::string& entity) {
    m_entity = entity;
}

void EdProximity::fetch(RestClient::Connection* connection) {
    //do nothing
}

void EdProximity::set_pos(int x, int y) {
    m_x = x;
    m_y = y;
}

int EdProximity::x() const {
    return m_x;
}

int EdProximity::y() const {
    return m_y;
}

int EdProximity::w() const {
    return 1800;
}

int EdProximity::h() const {
    return 600;
}

void EdProximity::render(SDL_Renderer* renderer) {
    //do nothing
}