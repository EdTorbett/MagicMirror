//
// Created by Edward Torbett on 09/12/2023.
//

#ifndef MAGICMIRROR_HOMEASSISTANT_H
#define MAGICMIRROR_HOMEASSISTANT_H


#include <string>
#include <connection.h>

#include "Calendar.h"
#include "Proximity.h"
#include "Forecast.h"

typedef enum Presence {
    PRESENCE_HOME = 0,
    PRESENCE_WORK,
    PRESENCE_OTHER
} Presence;

class HomeAssistant {
public:
    HomeAssistant();
    void update();
    void render(SDL_Renderer *renderer);
    [[nodiscard]] bool running() const { return m_running; }
    void stop() { m_running = false; }
private:
    void turn_off_display();
    void turn_on_display();

    std::chrono::time_point<std::chrono::steady_clock> m_last_full_fetch;
    std::chrono::time_point<std::chrono::steady_clock> m_last_user_fetch;
    std::chrono::time_point<std::chrono::steady_clock> m_last_user_change_time;
    std::chrono::time_point<std::chrono::steady_clock> m_last_presence_fetch;
    std::string m_url;
    std::string m_token;
    std::string m_ed_entity;
    std::string m_cath_entity;
    std::string m_user;
    Calendar m_calendar;
    Forecast m_forecast;
    RenderableText *m_welcome;
    RenderableText *m_date;
    RenderableText *m_clock;
    RestClient::Connection *m_connection;
    bool m_connected;
    bool m_display_is_on;
    Presence m_ed_presence;
    Presence m_cath_presence;
    Proximity m_ed_proximity;
    Proximity m_cath_proximity;
    bool m_running{};
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
