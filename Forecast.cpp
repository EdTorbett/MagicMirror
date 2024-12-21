//
// Created by Ed on 01/01/2024.
//

#include "Forecast.h"
#include "Line.h"
#include "WeatherEntry.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>

Forecast::Forecast() = default;

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
    std::vector<std::shared_ptr<WeatherEntry>> entries;
    nlohmann::json json_result = nlohmann::json::parse(r.body);
    split_weather(entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_2");
    json_result = nlohmann::json::parse(r.body);
    split_weather(entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_3");
    json_result = nlohmann::json::parse(r.body);
    split_weather(entries, json_result["state"]);
    r = connection->get("/api/states/input_text.hourly_weather_4");
    json_result = nlohmann::json::parse(r.body);
    split_weather(entries, json_result["state"]);

    int x_offset = 0;

    for (const auto& entry : entries) {
        add_child(entry, x_offset, 0);
        x_offset += entry->w();
    }

    add_child(std::make_shared<Line>(GREY, Forecast::w(), 0), 0, Forecast::h());
}

int Forecast::h() const {
    return 150;
}

int Forecast::w() const {
    return 1080;
}