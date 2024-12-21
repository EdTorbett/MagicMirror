//
// Created by Ed on 02/01/2024.
//

#include "Proximity.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>


Proximity::Proximity(const std::string &symbol): m_travelling(false), m_proximity(0), m_start_proximity(0), m_speed(0) {
    m_startSymbol = std::make_shared<RenderableText>("\uF3A3", 120, WHITE, FONTTYPE_SYMBOL, ALIGN_LEFT);
    m_endSymbol = std::make_shared<RenderableText>("\uF38F", 120, WHITE, FONTTYPE_SYMBOL, ALIGN_RIGHT);
    m_travellerSymbol = std::make_shared<RenderableText>(symbol, 120, WHITE, FONTTYPE_SYMBOL, ALIGN_CENTER);
    m_eta = std::make_shared<RenderableText>("ETA: Unknown", 40, WHITE, FONTTYPE_MONO, ALIGN_CENTER);
    add_child(m_startSymbol, -450, -80);
    add_child(m_endSymbol, 450, -80);
    add_child(m_travellerSymbol, 0, -80);
    add_child(m_eta, 0, 40);

    m_line1 = std::make_shared<Line>(WHITE, 350, 0);
    m_line2 = std::make_shared<Line>(WHITE, -350, 0);

    add_child(m_line1, -400, 60);
    add_child(m_line2, 400, 60);
}

Proximity::~Proximity() = default;


void Proximity::set_entity(const std::string& entity) {
    m_entity = entity;
}

void Proximity::fetch(RestClient::Connection* connection) {
    const auto& now = std::chrono::steady_clock::now();
    if (auto r = connection->get("/api/states/" + m_entity); r.code == 200) {
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

bool Proximity::is_interesting() const {
    return m_travelling;
}

int Proximity::w() const {
    return 1000;
}

int Proximity::h() const {
    return 200;
}

void Proximity::render(SDL_Renderer* renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) {
    if (skip_render(now)) {
        return;
    }
    std::ostringstream oss;
    if (m_proximity > 1000) {
        oss << std::setprecision(2) << std::fixed << (m_proximity / 1000.0) << "km away, ";
    } else {
        oss << m_proximity << "m away, ";
    }
    const int traveller_position = 800 * (m_start_proximity - m_proximity) / m_start_proximity;
    m_travellerSymbol->set_pos(x() - 400 + traveller_position, y() - 80);

    if (m_speed > 0) {
        const auto time_since_poll = now - m_poll_time;
        const auto time_taken = std::chrono::seconds(static_cast<int>(m_proximity / m_speed));
        const auto expected_remaining_time = time_taken - time_since_poll;
        const auto &arrival_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + expected_remaining_time);

        std::string s(30, '\0');
        std::strftime(&s[0], s.size(), "ETA %H:%M:%S", std::localtime(&arrival_time));
        m_eta->set_text(s);
    } else {
        // Unknown ETA
        m_eta->set_text("ETA unknown");
    }

    if (traveller_position > 150) {
        m_line1->set_dx(traveller_position - 150);
    } else {
        m_line1->set_dx(0);
    }
    if (traveller_position < 850) {
        m_line2->set_dx(-850 + traveller_position);
    } else {
        m_line2->set_dx(0);
    }

    Renderable::render(renderer, now);
}