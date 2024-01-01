//
// Created by Edward Torbett on 09/12/2023.
//

#ifndef MAGICMIRROR_HOMEASSISTANT_H
#define MAGICMIRROR_HOMEASSISTANT_H


#include <string>
#include <connection.h>
#include <memory>

#include "Calendar.h"
#include "Forecast.h"
#include "WeatherEntry.h"

class HomeAssistant {
public:
    HomeAssistant();
    void fetch();
    void render(SDL_Renderer *renderer);
private:
    std::string m_url;
    std::string m_token;
    Calendar m_calendar;
    Forecast m_forecast;
    RenderableText *m_date;
    RenderableText *m_clock;
    RestClient::Connection *m_connection;
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
