//
// Created by Ed on 02/01/2024.
//

#include "Proximity.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>


Proximity::Proximity(const std::string &symbol): m_travelling(false), m_proximity(0), m_start_time(0), m_poll_time(0), m_start_proximity(0), m_x(0), m_y(0), m_speed(0) {
    m_startSymbol = new RenderableText("\uF3A3", 120, WHITE, FONTTYPE_SYMBOL, ALIGN_LEFT);
    m_endSymbol = new RenderableText("\uF38F", 120, WHITE, FONTTYPE_SYMBOL, ALIGN_RIGHT);
    m_travellerSymbol = new RenderableText(symbol, 120, WHITE, FONTTYPE_SYMBOL, ALIGN_CENTER);
    m_eta = new RenderableText("ETA: Unknown", 40, WHITE, FONTTYPE_MONO, ALIGN_CENTER);
}

Proximity::~Proximity() = default;


void Proximity::set_entity(const std::string& entity) {
    m_entity = entity;
}

void Proximity::fetch(RestClient::Connection* connection) {
    time_t now;
    time(&now);
    auto r = connection->get("/api/states/" + m_entity);
    if (r.code == 200) {
        nlohmann::json json_result = nlohmann::json::parse(r.body);
        const std::string proximity_str = json_result["state"];
        m_proximity = std::stoi(proximity_str);
        nlohmann::json &attributes = json_result["attributes"];

        m_travelling = attributes["dir_of_travel"] == "towards";
        m_poll_time = now;

        if (!m_travelling || !m_start_proximity) {
            m_start_proximity = m_proximity;
            m_start_time = now;
            m_speed = 0;
        } else if (m_proximity < m_start_proximity && now > m_start_time){
            const time_t time_since_start = now - m_start_time;
            const int distance_travelled = m_start_proximity - m_proximity;
            m_speed = 1.0 * distance_travelled / time_since_start;
        } else {
            m_speed = 0;
        }
    } else {
        m_travelling = false;
    }
}

void Proximity::set_pos(int x, int y) {
    m_x = x - 600;
    m_y = y - 80;
    m_startSymbol->set_pos(x - 600, m_y);
    m_endSymbol->set_pos(x + 600, m_y);
    m_travellerSymbol->set_pos(x, m_y);
    m_eta->set_pos(x, m_y + 120);
}

bool Proximity::is_interesting() const {
    return m_travelling;
}

int Proximity::x() const {
    return m_x;
}

int Proximity::y() const {
    return m_y;
}

int Proximity::w() const {
    return 1200;
}

int Proximity::h() const {
    return 160;
}

void Proximity::render(SDL_Renderer* renderer) {
    if (!is_interesting()) {
        return;
    }
    time_t now;
    time(&now);
    std::ostringstream oss;
    if (m_proximity > 1000) {
        oss << std::setprecision(2) << std::fixed << (m_proximity / 1000.0) << "km away, ";
    } else {
        oss << m_proximity << "m away, ";
    }
    const int traveller_position = 900 * (m_start_proximity - m_proximity) / m_start_proximity;
    if (m_speed > 0) {
        const auto time_since_poll = now - m_poll_time;
        const auto expected_remaining_time = std::max(static_cast<time_t>(0), static_cast<time_t>(m_proximity / m_speed) - time_since_poll);
        tm arrival_time({0});
        arrival_time = *gmtime(&expected_remaining_time);
        oss << std::put_time(&arrival_time, "ETA %H:%M:%S");
        m_eta->set_text(oss.str());
        m_travellerSymbol->set_pos(m_x + 150 + traveller_position, m_y);
    } else {
        // Unknown ETA
        m_eta->set_text("ETA unknown");
    }

    m_startSymbol->render(renderer);
    m_endSymbol->render(renderer);
    m_travellerSymbol->render(renderer);
    m_eta->render(renderer);

    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
    if (traveller_position > 150) {
        SDL_RenderDrawLine(renderer, m_x + 150, m_y + 60, m_x + 50 + traveller_position, m_y + 60);
    }
    if (traveller_position < 750) {
        SDL_RenderDrawLine(renderer, m_x + 250 + traveller_position, m_y + 60, m_x + 1050, m_y + 60);
    }
}