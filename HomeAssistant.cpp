//
// Created by Edward Torbett on 09/12/2023.
//

#include <fstream>
#include <sstream>
#include <chrono>
#include "HomeAssistant.h"

#include <iomanip>
#include <iostream>
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
    m_last_full_fetch(0),
    m_last_user_fetch(0),
    m_date(nullptr),
    m_clock(nullptr),
    m_connection(nullptr)
{
    RestClient::init();

    m_calendar.set_pos(60, 140);
    m_forecast.set_pos(60, 20);

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

    m_date = new RenderableText("Date goes here", 80, WHITE, FONTTYPE_REGULAR, ALIGN_RIGHT);
    m_clock = new RenderableText("00:00", 80, WHITE, FONTTYPE_MONO, ALIGN_RIGHT);
}

void HomeAssistant::update(CEC::ICECAdapter* cec_adapter) {
    time_t now;
    time(&now);

    if (now - m_last_user_fetch >= 5) {
        RestClient::Response r = m_connection->get("/api/states/input_select.mirror_user");
        nlohmann::json json_result = nlohmann::json::parse(r.body);
        if (m_user != json_result["state"]) {
            m_user = json_result["state"];
            std::cout << "User changed: " << m_user << std::endl;
            if (m_user == "Nobody") {
                // Turn off mirror
                cec_adapter->StandbyDevices();
            } else {
                // Turn on mirror
                cec_adapter->PowerOnDevices();
            }
        }

        m_last_user_fetch = now;
    }

    if (now - m_last_full_fetch >= 300) {
        m_calendar.fetch(m_connection);
        m_forecast.fetch(m_connection);
    }
}

void HomeAssistant::render(SDL_Renderer *renderer) {
    this->m_calendar.render(renderer);
    this->m_forecast.render(renderer);

    time_t rawtime;
    time ( &rawtime );
    const tm* timeinfo = localtime(&rawtime);
    std::ostringstream os;
    os << DAYS_OF_WEEK[timeinfo->tm_wday] << " " << std::setw(2) << timeinfo->tm_mday << " " << MONTHS[timeinfo->tm_mon];
    m_date->set_text(os.str());
    m_date->set_pos(1920 - 60, 1080 - 20 - m_date->h());
    m_date->render(renderer);

    os.str("");
    os.clear();
    os << std::put_time(timeinfo, "%H:%M");
    m_clock->set_text(os.str());
    m_clock->set_pos(1920 - 60, 1080 + 10 - m_clock->h() - m_date->h());
    m_clock->render(renderer);
}
