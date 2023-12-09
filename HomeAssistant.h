//
// Created by Edward Torbett on 09/12/2023.
//

#ifndef MAGICMIRROR_HOMEASSISTANT_H
#define MAGICMIRROR_HOMEASSISTANT_H


#include <string>
#include <vector>
#include <connection.h>
#include <restclient.h>

class HomeAssistant {
public:
    HomeAssistant();
    std::vector<std::string> getCalendar();
private:
    std::string m_url;
    std::string m_token;
    RestClient::Connection *m_connection;
};


#endif //MAGICMIRROR_HOMEASSISTANT_H
