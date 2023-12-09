//
// Created by Edward Torbett on 09/12/2023.
//

#include <iostream>
#include <fstream>
#include "HomeAssistant.h"
#include <nlohmann/json.hpp>


HomeAssistant::HomeAssistant() :
    m_token(),
    m_connection(nullptr)
{
    RestClient::init();

    std::ifstream token_file("../token");
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
}

std::vector<std::string> HomeAssistant::getCalendar() {
    std::vector<std::string> retval;

    RestClient::Response r = m_connection->get("/api/calendars/calendar.shared_calendar?start=2023-12-09T00:00:00.000Z&end=2024-01-09T00:00:00.000Z");
    nlohmann::json json_result = nlohmann::json::parse(r.body);

    std::cout << std::setw(2) << json_result;

    return retval;
}
