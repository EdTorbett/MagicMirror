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
#include <unistd.h>
#include <pwd.h>

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
    m_last_presence_fetch(0),
    m_date(nullptr),
    m_clock(nullptr),
    m_connection(nullptr),
    m_connected(false),
    m_ed_presence(PRESENCE_OTHER),
    m_cath_presence(PRESENCE_OTHER)
{
    RestClient::init();

    m_calendar.set_pos(60, 140);
    m_forecast.set_pos(60, 20);

    std::string homedir;

    if (getenv("HOME") == nullptr) {
        homedir = getpwuid(getuid())->pw_dir;
    } else {
        homedir = getenv("HOME");
    }

    std::ifstream token_file(homedir + "/.token");
    std::string tmp;

    if (token_file.is_open())
    {
        getline(token_file, m_url);
        getline(token_file, m_token);
        getline(token_file, m_ed_entity);
        getline(token_file, m_cath_entity);
        getline(token_file, tmp);
        m_ed_proximity.set_entity(tmp);
        getline(token_file, tmp);
        m_cath_proximity.set_entity(tmp);
        token_file.close();
    }

    m_date = new RenderableText("Date goes here", 80, WHITE, FONTTYPE_REGULAR, ALIGN_RIGHT);
    m_clock = new RenderableText("00:00", 80, WHITE, FONTTYPE_MONO, ALIGN_RIGHT);
}

void HomeAssistant::update(CEC::ICECAdapter* cec_adapter) {
    time_t now;
    time(&now);

    if (now - m_last_user_fetch >= 5) {
        if (m_connection == nullptr) {
            m_connection = new RestClient::Connection(m_url);
            RestClient::HeaderFields headers;
            headers["Content-Type"] = "application/json";
            headers["Authorization"] = m_token;
            m_connection->SetHeaders(headers);
            m_connection->SetTimeout(10);
        }

        RestClient::Response r = m_connection->get("/api/states/input_select.mirror_user");
        if (r.code == 200) {
            nlohmann::json json_result = nlohmann::json::parse(r.body);
            std::string new_user = json_result["state"];
            if (m_user != new_user) {
                std::cout << "User changed: " << new_user << std::endl;
                if (new_user == "Nobody") {
                    // Turn off mirror
                    std::cout << "Putting display into standby..." << std::endl;
                    cec_adapter->StandbyDevices(CEC::CECDEVICE_TV);
                } else if (m_user == "Nobody") {
                    // Turn on mirror
                    std::cout << "Waking display..." << std::endl;
                    cec_adapter->PowerOnDevices(CEC::CECDEVICE_TV);
                }
                m_user = new_user;
            }
            m_connected = true;
        } else {
            m_connected = false;
        }
        m_last_user_fetch = now;
    }

    if (m_connected && now - m_last_presence_fetch >= 30) {
        m_ed_proximity.fetch(m_connection);
        m_cath_proximity.fetch(m_connection);
        RestClient::Response r = m_connection->get("/api/states/" + m_ed_entity);
        if (r.code == 200) {
            nlohmann::json json_result = nlohmann::json::parse(r.body);
            Presence new_state = PRESENCE_OTHER;
            if (json_result["state"] == "home") {
                new_state = PRESENCE_HOME;
            } else if (json_result["state"] == "work") {
                new_state = PRESENCE_WORK;
            }
            m_ed_presence = new_state;
        }
        r = m_connection->get("/api/states/" + m_cath_entity);
        if (r.code == 200) {
            nlohmann::json json_result = nlohmann::json::parse(r.body);
            Presence new_state = PRESENCE_OTHER;
            if (json_result["state"] == "home") {
                new_state = PRESENCE_HOME;
            } else if (json_result["state"] == "work") {
                new_state = PRESENCE_WORK;
            }
            m_cath_presence = new_state;
        }
        m_last_presence_fetch = now;
    }

    if (m_connected && now - m_last_full_fetch >= 300) {
        m_calendar.fetch(m_connection);
        m_forecast.fetch(m_connection);
        m_last_full_fetch = now;
    }
}

void HomeAssistant::render(SDL_Renderer *renderer) {
    if (m_user == "Nobody") {
        return;
    }
    if (m_user != "Unknown") {
        this->m_calendar.render(renderer);
    }
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
