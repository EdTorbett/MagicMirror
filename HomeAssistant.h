//
// Created by Edward Torbett on 09/12/2023.
//

#ifndef MAGICMIRROR_HOMEASSISTANT_H
#define MAGICMIRROR_HOMEASSISTANT_H


#include <string>
#include <connection.h>
#include <memory>
#include <cec.h>

#include "Calendar.h"
#include "CathProximity.h"
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
    void update(CEC::ICECAdapter* cec_adapter);
    void render(SDL_Renderer *renderer);
private:
    void turn_off_display(CEC::ICECAdapter* cec_adapter, time_t &now);
    void turn_on_display(CEC::ICECAdapter* cec_adapter, time_t &now);

    time_t m_last_full_fetch;
    time_t m_last_user_fetch;
    time_t m_last_presence_fetch;
    time_t m_last_cec_command;
    int m_last_cec_command_type;
    std::string m_url;
    std::string m_token;
    std::string m_ed_entity;
    std::string m_cath_entity;
    std::string m_user;
    Calendar m_calendar;
    Forecast m_forecast;
    RenderableText *m_date;
    RenderableText *m_clock;
    RestClient::Connection *m_connection;
    bool m_connected;
    Presence m_ed_presence;
    Presence m_cath_presence;
    Proximity m_ed_proximity;
    Proximity m_cath_proximity;
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
