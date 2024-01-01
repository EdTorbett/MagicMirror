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
#include "Forecast.h"
#include "WeatherEntry.h"

class HomeAssistant {
public:
    HomeAssistant();
    void update(CEC::ICECAdapter* cec_adapter);
    void render(SDL_Renderer *renderer);
private:
    time_t m_last_full_fetch;
    time_t m_last_user_fetch;
    std::string m_url;
    std::string m_token;
    std::string m_user;
    Calendar m_calendar;
    Forecast m_forecast;
    RenderableText *m_date;
    RenderableText *m_clock;
    RestClient::Connection *m_connection;
    bool m_connected;
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
