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
#include <cstdlib>
#include <filesystem>

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
    m_welcome(nullptr),
    m_date(nullptr),
    m_clock(nullptr),
    m_video(nullptr),
    m_connection(nullptr),
    m_ed_proximity("\uF369"),
    m_cath_proximity("\uF36F"),
    m_living_room_in_use(true),
    m_running(true),
    m_random_time(300, 86400)
{

    std::random_device source;
    std::random_device::result_type random_data[(std::mt19937::state_size - 1) / sizeof(source()) + 1];
    std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
    std::seed_seq seeds(std::begin(random_data), std::end(random_data));
    m_random_generator.seed(seeds);

    RestClient::init();

    m_calendar.set_pos(0, 200);
    m_forecast.set_pos(0, 0);

    m_ed_proximity.set_pos(540, 960);
    m_cath_proximity.set_pos(540, 960);

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
    m_welcome = new RenderableText("", 80, WHITE, FONTTYPE_REGULAR, ALIGN_CENTER);
    m_welcome->set_pos(540, 960);
    m_welcome->set_transient(true);

    m_next_hidden_appearance = std::chrono::steady_clock::now() + std::chrono::seconds(m_random_time(m_random_generator));

    m_video = new RenderableVideo();
    m_video->set_pos(540, 960);

    system("ddcutil setvcp 10 0");
}

void HomeAssistant::update() {
    const auto& now = std::chrono::steady_clock::now();

    if (now - m_last_user_fetch >= std::chrono::seconds(5)) {
        if (m_connection == nullptr) {
            m_connection = new RestClient::Connection(m_url);
            RestClient::HeaderFields headers;
            headers["Content-Type"] = "application/json";
            headers["Authorization"] = m_token;
            m_connection->SetHeaders(headers);
            m_connection->SetTimeout(10);
        }

        RestClient::Response r = m_connection->get("/api/states/binary_sensor.living_room_in_use");
        if (r.code == 200) {
            nlohmann::json json_result = nlohmann::json::parse(r.body);
            m_living_room_in_use = (json_result["state"] == "on");
        } else {
            m_living_room_in_use = false;
        }

        r = m_connection->get("/api/states/input_select.mirror_user");
        if (r.code == 200) {
            nlohmann::json json_result = nlohmann::json::parse(r.body);
            std::string new_user = json_result["state"];
            if (m_user != new_user) {
                std::cout << "User changed: " << new_user << std::endl;
                m_user = new_user;
                if (m_user != "Nobody" && m_user != "Unknown") {
                    m_welcome->set_text("Welcome, " + m_user + "!");
                    m_welcome->set_visible(true, now);
                }
            }
        }
        m_last_user_fetch = now;
    }

    if (now - m_last_full_fetch >= std::chrono::seconds(300)) {
        m_calendar.fetch(m_connection);
        m_forecast.fetch(m_connection);
        m_ed_proximity.fetch(m_connection);
        m_cath_proximity.fetch(m_connection);
        m_last_full_fetch = now;
    }

    if (m_living_room_in_use) {
        m_clock->set_visible(true, now);
        m_date->set_visible(true, now);

        if (m_ed_proximity.is_interesting()) {
            m_ed_proximity.set_visible(true, now);
        } else if (m_cath_proximity.is_interesting()) {
            m_cath_proximity.set_visible(true, now);
        }

        if (m_user == "Nobody") {
            this->m_calendar.set_visible(false, now);
            this->m_forecast.set_visible(false, now);
        } else if (m_user == "Unknown") {
            this->m_calendar.set_visible(false, now);
            this->m_forecast.set_visible(true, now);
        } else {
            this->m_calendar.set_visible(true, now);
            this->m_forecast.set_visible(true, now);
        }

        if (m_video->finished() && !m_video->visible()) {
            if (now > m_next_hidden_appearance) {
                std::cout << "Video launch" << std::endl;
                m_video->initialise("file:/home/magicmirror/ui/movies/mirror.webm");
                m_next_hidden_appearance = std::chrono::steady_clock::now() + std::chrono::seconds(m_random_time(m_random_generator));
            }
        }
    } else {
        m_clock->set_visible(false, now);
        m_date->set_visible(false, now);
        m_calendar.set_visible(false, now);
        m_forecast.set_visible(false, now);
        m_ed_proximity.set_visible(false, now);
        m_cath_proximity.set_visible(false, now);
    }

    if (m_video->finished() && m_video->visible()) {
        m_video->teardown();
    }
}

void HomeAssistant::render(SDL_Renderer *renderer) {
    const auto& now = std::chrono::steady_clock::now();

    m_video->render(renderer, now);
    m_welcome->render(renderer, now);
    this->m_calendar.render(renderer, now);
    this->m_forecast.render(renderer, now);
    this->m_ed_proximity.render(renderer, now);
    this->m_cath_proximity.render(renderer, now);

    time_t rawtime;
    time ( &rawtime );
    const tm* timeinfo = localtime(&rawtime);
    std::ostringstream os;
    os << DAYS_OF_WEEK[timeinfo->tm_wday] << " " << std::setw(2) << timeinfo->tm_mday << " " << MONTHS[timeinfo->tm_mon];
    m_date->set_text(os.str());
    m_date->set_pos(1080, 1920 - m_date->h());
    m_date->render(renderer, now);

    os.str("");
    os.clear();
    os << std::put_time(timeinfo, "%H:%M");
    m_clock->set_text(os.str());
    m_clock->set_pos(1080, 1920 - m_clock->h() - m_date->h());
    m_clock->render(renderer, now);
}
