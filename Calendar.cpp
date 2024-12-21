//
// Created by Ed on 01/01/2024.
//

#include "Calendar.h"
#include <sstream>
#include <nlohmann/json.hpp>

Calendar::Calendar() = default;

using days = std::chrono::duration<int64_t, std::ratio<86400>>;

void Calendar::fetch(RestClient::Connection *connection) {
    auto const time = floor<days>(std::chrono::system_clock::now());
    std::time_t today = std::chrono::system_clock::to_time_t(time);
    std::string cal_start(30, '\0');
    cal_start.resize(std::strftime(&cal_start[0], cal_start.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&today)));
    today += (86400 * 60);
    std::string cal_end(30, '\0');
    cal_end.resize(std::strftime(&cal_end[0], cal_end.size(), "%Y-%m-%dT00:00:00.000Z", std::localtime(&today)));
    std::stringstream ss;
    ss << "/api/calendars/calendar.shared_calendar?start=" << cal_start << "&end=" << cal_end;
    RestClient::Response r = connection->get(ss.str());
    if (r.code != 200) {
        return;
    }
    nlohmann::json json_result = nlohmann::json::parse(r.body);

    clear_children();

    int count = 0;

    std::vector<std::shared_ptr<CalendarEntry>> entries;

    for (const auto& item : json_result) {
        entries.push_back(std::make_shared<CalendarEntry>(item, time));
        if (++count == 10) {
            break;
        }
    }

    int x_offset = 50;
    int y_offset = 0;

    const unsigned int split = (entries.size() + 1) / 2;
    unsigned int i = 0;
    for (const auto& entry : entries) {
        add_child(entry, x_offset, y_offset);
        y_offset += entry->h();
        i += 1;
        if (i == split) {
            x_offset = 590;
            y_offset = 0;
        }
    }
}
