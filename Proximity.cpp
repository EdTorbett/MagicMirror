//
// Created by Ed on 02/01/2024.
//

#include "Proximity.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>


Proximity::Proximity(const std::string &symbol): m_travelling(false), m_proximity(0), m_start_proximity(0), m_speed(0), m_x(0), m_y(0) {
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
    const auto& now = std::chrono::steady_clock::now();
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
            const double time_since_start = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start_time).count()) / 1000.0;
            const int distance_travelled = m_start_proximity - m_proximity;
            m_speed = distance_travelled / time_since_start;
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

void Proximity::render(SDL_Renderer* renderer, float brightness) {
    if (!is_interesting()) {
        return;
    }
    const auto &now = std::chrono::steady_clock::now();
    std::ostringstream oss;
    if (m_proximity > 1000) {
        oss << std::setprecision(2) << std::fixed << (m_proximity / 1000.0) << "km away, ";
    } else {
        oss << m_proximity << "m away, ";
    }
    const int traveller_position = 900 * (m_start_proximity - m_proximity) / m_start_proximity;
    if (m_speed > 0) {
        const auto time_since_poll = now - m_poll_time;
        const auto time_taken = std::chrono::seconds(static_cast<int>(m_proximity / m_speed));
        const auto expected_remaining_time = time_taken - time_since_poll;
        const auto &arrival_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + expected_remaining_time);

        std::string s(30, '\0');
        std::strftime(&s[0], s.size(), "ETA %H:%M:%S", std::localtime(&arrival_time));

        m_eta->set_text(s);
        m_travellerSymbol->set_pos(m_x + 150 + traveller_position, m_y);
    } else {
        // Unknown ETA
        m_eta->set_text("ETA unknown");
    }

    m_startSymbol->render(renderer, brightness);
    m_endSymbol->render(renderer, brightness);
    m_travellerSymbol->render(renderer, brightness);
    m_eta->render(renderer, brightness);

    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, static_cast<uint8_t>(0xFF * brightness));
    if (traveller_position > 150) {
        SDL_RenderDrawLine(renderer, m_x + 150, m_y + 60, m_x + 50 + traveller_position, m_y + 60);
    }
    if (traveller_position < 750) {
        SDL_RenderDrawLine(renderer, m_x + 250 + traveller_position, m_y + 60, m_x + 1050, m_y + 60);
    }
}