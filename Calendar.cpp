//
// Created by Ed on 01/01/2024.
//

#include "Calendar.h"
#include <sstream>
#include <nlohmann/json.hpp>

Calendar::Calendar() = default;

Calendar::~Calendar() {
    this->m_entries.clear();
}

using days = std::chrono::duration<int64_t, std::ratio<86400>>;

void Calendar::fetch(RestClient::Connection *connection) {
    auto const time = floor<days>(std::chrono::system_clock::now());
    std::time_t today = std::chrono::system_clock::to_time_t(time);
    std::string cal_start(30, '\0');
    cal_start.resize(std::strftime(&cal_start[0], cal_start.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&today)));
    today += (86400 * 60);
    std::string cal_end(30, '\0');
    cal_end.resize(std::strftime(&cal_end[0], cal_end.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&today)));
    std::stringstream ss;
    ss << "/api/calendars/calendar.shared_calendar?start=" << cal_start << "&end=" << cal_end;
    RestClient::Response r = connection->get(ss.str());
    if (r.code != 200) {
        return;
    }
    nlohmann::json json_result = nlohmann::json::parse(r.body);

    this->m_entries.clear();

    int count = 0;

    for (const auto& item : json_result) {
        auto *entry = new CalendarEntry(item, time);
        this->m_entries.emplace_back(entry);
        if (++count == 10) {
            break;
        }
    }

    update_entry_positions();
}

void Calendar::set_pos(int x, int y) {
    this->m_x = x;
    this->m_y = y;
    update_entry_positions();
}

void Calendar::update_entry_positions() {
    int x_offset = this->m_x + 50;
    int y_offset = this->m_y;

    const unsigned int split = (this->m_entries.size() + 1) / 2;
    unsigned int i = 0;
    for (const auto& entry : this->m_entries) {
        entry->set_pos(x_offset, y_offset);
        y_offset += entry->h();
        i += 1;
        if (i == split) {
            x_offset += 540;
            y_offset = this->m_y;
        }
    }
}

void Calendar::render(SDL_Renderer* renderer, float brightness) {
    for (const auto& entry : this->m_entries) {
        entry->render(renderer, brightness);
    }
}

int Calendar::x() const {
    return this->m_x;
}

int Calendar::y() const {
    return this->m_y;
}

int Calendar::h() const {
    int h = 0;
    for (const auto& entry : this->m_entries) {
        h += entry->h();
    }
    return h;
}

int Calendar::w() const {
    int w = 0;
    for (const auto& entry : this->m_entries) {
        if (entry->w() > w) {
            w = entry->w();
        }
    }
    return w;
}
