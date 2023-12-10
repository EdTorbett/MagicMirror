//
// Created by Edward Torbett on 09/12/2023.
//

#ifndef MAGICMIRROR_HOMEASSISTANT_H
#define MAGICMIRROR_HOMEASSISTANT_H


#include "Renderable.h"
#include <string>
#include <vector>
#include <connection.h>
#include <restclient.h>
#include "CalendarEntry.h"

class HomeAssistant {
public:
    HomeAssistant();
    void fetchCalendar();
    void render(SDL_Renderer *renderer);
private:
    std::string m_url;
    std::string m_token;
    std::vector<std::shared_ptr<CalendarEntry>> m_calendar;
    RestClient::Connection *m_connection;
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
