//
// Created by Edward Torbett on 09/12/2023.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "HomeAssistant.h"
#include <nlohmann/json.hpp>

HomeAssistant::HomeAssistant() :
    m_token(),
    m_connection(nullptr)
{
    RestClient::init();

    std::ifstream token_file("../token");
    if (token_file.is_open())
    {
        getline(token_file,m_url);
        getline(token_file,m_token);
        token_file.close();
    }

    m_connection = new RestClient::Connection(m_url);
    RestClient::HeaderFields headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = m_token;
    m_connection->SetHeaders(headers);
    m_connection->SetTimeout(10);
}

void HomeAssistant::fetchCalendar() {
    auto const time = std::chrono::system_clock::now();
    std::time_t now =     std::chrono::system_clock::to_time_t(time);
    std::string cal_start(30, '\0');
    cal_start.resize(std::strftime(&cal_start[0], cal_start.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&now)));
    now += (86400 * 30);
    std::string cal_end(30, '\0');
    cal_end.resize(std::strftime(&cal_end[0], cal_end.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&now)));
    std::stringstream ss;
    ss << "/api/calendars/calendar.shared_calendar?start=" << cal_start << "&end=" << cal_end;
    RestClient::Response r = m_connection->get(ss.str());
    nlohmann::json json_result = nlohmann::json::parse(r.body);

    this->m_calendar.clear();

    int y_offset = 0;
    int count = 0;

    for (const auto& item : json_result) {
        auto *entry = new CalendarEntry(item, time);
        entry->set_pos(0, y_offset);
        y_offset += entry->h();
        this->m_calendar.emplace_back(entry);
        if (++count == 10) {
            break;
        }
    }
}

void HomeAssistant::render(SDL_Renderer *renderer) {
    for (auto& entry : this->m_calendar) {
        entry->render(renderer);
    }
}
