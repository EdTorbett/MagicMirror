//
// Created by Edward Torbett on 09/12/2023.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "HomeAssistant.h"

#include <iomanip>
#include <nlohmann/json.hpp>
#include <restclient.h>

#include "WeatherEntry.h"

const std::string DAYS_OF_WEEK[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const std::string MONTHS[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

HomeAssistant::HomeAssistant() :
    m_url(),
    m_token(),
    m_date(nullptr),
    m_clock(nullptr),
    m_connection(nullptr)
{
    RestClient::init();

    std::ifstream token_file("../token");
    std::string tmp;

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

    m_date = new RenderableText("Date goes here", 80, WHITE, FONTTYPE_MONO);
    m_clock = new RenderableText("00:00:00", 80, WHITE, FONTTYPE_MONO);
}

void HomeAssistant::fetchCalendar() {
    auto const time = std::chrono::system_clock::now();
    std::time_t now = std::chrono::system_clock::to_time_t(time);
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

    int y_offset = 100;
    int count = 0;

    for (const auto& item : json_result) {
        auto *entry = new CalendarEntry(item, time);
        entry->set_pos(60, y_offset);
        y_offset += entry->h();
        this->m_calendar.emplace_back(entry);
        if (++count == 10) {
            break;
        }
    }
}

void split_weather (std::vector<std::shared_ptr<WeatherEntry>> &result, const std::string &s) {
    std::stringstream ss(s);
    std::string item;

    int x_offset = 60 + static_cast<int>(150 * result.size());

    while (getline (ss, item, ';')) {
        auto *entry = new WeatherEntry(item);
        entry->set_pos(x_offset, 20);
        x_offset += entry->w();
        result.emplace_back(entry);
    }
}

void HomeAssistant::fetchForecast() {
    RestClient::Response r = m_connection->get("/api/states/input_text.hourly_weather_1");
    m_forecast.clear();
    nlohmann::json json_result = nlohmann::json::parse(r.body);
    split_weather(m_forecast, json_result["state"]);
    r = m_connection->get("/api/states/input_text.hourly_weather_2");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_forecast, json_result["state"]);
    r = m_connection->get("/api/states/input_text.hourly_weather_3");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_forecast, json_result["state"]);
    r = m_connection->get("/api/states/input_text.hourly_weather_4");
    json_result = nlohmann::json::parse(r.body);
    split_weather(m_forecast, json_result["state"]);
}

void HomeAssistant::render(SDL_Renderer *renderer) {
    for (auto& entry : this->m_calendar) {
        entry->render(renderer);
    }
    for (auto& entry : this->m_forecast) {
        entry->render(renderer);
    }

    time_t rawtime;
    time ( &rawtime );
    const tm* timeinfo = localtime(&rawtime);
    std::ostringstream os;
    os << DAYS_OF_WEEK[timeinfo->tm_wday] << " " << std::setw(2) << timeinfo->tm_mday << " " << MONTHS[timeinfo->tm_mon];
    m_date->set_text(os.str());
    m_date->set_pos(1920 - 60 - m_date->w(), 1080 - 20 - m_date->h());
    m_date->render(renderer);

    os.str("");
    os.clear();
    os << std::put_time(timeinfo, "%H:%M:%S");
    m_clock->set_text(os.str());
    m_clock->set_pos(1920 - 60 - m_clock->w(), 1080 - 20 - m_clock->h() - m_date->h());
    m_clock->render(renderer);
}
