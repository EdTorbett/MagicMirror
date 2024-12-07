//
// Created by Ed on 01/01/2024.
//

#include "Forecast.h"
#include <sstream>
#include <nlohmann/json.hpp>

Forecast::Forecast() = default;

Forecast::~Forecast() {
    this->m_entries.clear();
}

void split_weather(std::vector<std::shared_ptr<WeatherEntry>> &result, const std::string &s) {
    std::stringstream ss(s);
    std::string item;

    while (getline (ss, item, ';')) {
        result.emplace_back(new WeatherEntry(item));
    }
}

void Forecast::fetch(RestClient::Connection *connection) {
    RestClient::Response r = connection->get("/api/states/input_text.hourly_weather_1");
    if (r.code != 200) {
        return;
    }
    m_entries.clear();
    nlohmann::json json_result = nlohmann::json::parse(r.body);
    split_weather(m_entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_2");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_3");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_4");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_entries, json_result["state"]);
    this->update_entry_positions();
}

void Forecast::update_entry_positions() {
    int x_offset = this->m_x;
    const int y_offset = this->m_y;

    for (const auto& entry : this->m_entries) {
        entry->set_pos(x_offset, y_offset);
        x_offset += entry->w();
    }
}

void Forecast::set_pos(int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->update_entry_positions();
}

void Forecast::render(SDL_Renderer* renderer) {
    for (const auto& entry : this->m_entries) {
        entry->render(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_RenderDrawLine(renderer, this->y() + this->h(), 1080 - this->x(), this->y() + this->h(), 1080 - (this->x() + this->w()));
}

int Forecast::x() const {
    return this->m_x;
}

int Forecast::y() const {
    return this->m_y;
}

int Forecast::h() const {
    return 150;
}

int Forecast::w() const {
    return 1080;
}