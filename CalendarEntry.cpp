//
// Created by Edward Torbett on 10/12/2023.
//

#include "CalendarEntry.h"
#include "RenderableText.h"
#include "Line.h"
#include <sstream>

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

const std::string START_KEY = "start";
const std::string END_KEY = "end";
const std::string DATE_KEY = "date";
const std::string DATETIME_KEY = "dateTime";

CalendarEntry::CalendarEntry(const nlohmann::json &entry, const std::chrono::system_clock::time_point &today) {
    RenderableText *date = nullptr;
    RenderableText *time = nullptr;
    RenderableText *description = nullptr;
    RenderableText *daysRemaining = nullptr;

    time_t event_start_time_t;
    tm event_time({0});
    std::ostringstream timeStringStream("");
    std::ostringstream dateStringStream("");
    if (entry.contains("start")) {
        const nlohmann::json &start = entry[START_KEY];
        if (start.contains(DATETIME_KEY)) {
            std::istringstream ss(std::string(start[DATETIME_KEY]).substr(0, 10));
            ss >> std::get_time(&event_time, "%Y-%m-%d");
            timeStringStream << std::string(start[DATETIME_KEY]).substr(11, 5);
        } else if (start.contains(DATE_KEY)) {
            std::string d = start[DATE_KEY];
            std::istringstream ss(d);
            ss >> std::get_time(&event_time, "%Y-%m-%d");
        }
        event_start_time_t = mktime(&event_time);
        event_time = *localtime(&event_start_time_t);

        dateStringStream.str("");
        dateStringStream.clear();
        dateStringStream << DAYS_OF_WEEK[event_time.tm_wday] << " " << std::setw(2) << event_time.tm_mday << " " << MONTHS[event_time.tm_mon];

        auto event_date = std::chrono::system_clock::from_time_t(event_start_time_t);
        auto days_till_event = floor<std::chrono::hours>(event_date - today).count() / 24;

        std::ostringstream daysToEvent;
        if (days_till_event == 0) {
            daysToEvent << "Today";
        } else if (days_till_event == 1) {
            daysToEvent << "Tomorrow";
        } else if (days_till_event == -1) {
            daysToEvent << "Started yesterday";
        } else if (days_till_event < 0) {
            daysToEvent << "Started " << -days_till_event << " days ago";
        } else {
            daysToEvent << days_till_event << " days time";
        }

        daysRemaining = new RenderableText(daysToEvent.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_RIGHT);
    }

    if (entry.contains(END_KEY) && entry[END_KEY].contains(DATE_KEY)) {
        std::string d = entry[END_KEY][DATE_KEY];
        std::istringstream ss(d);
        ss >> std::get_time(&event_time, "%Y-%m-%d");
        time_t event_end_time_t = mktime(&event_time);
        std::ostringstream oss;
        if (event_end_time_t == event_start_time_t + 86400) {
            timeStringStream.clear();
            timeStringStream << "All day";
        } else if (event_end_time_t > event_start_time_t) {
            event_end_time_t -= 1;
            event_time = *localtime(&event_end_time_t);
            dateStringStream << " to " << DAYS_OF_WEEK[event_time.tm_wday] << " " << std::setw(2) << event_time.tm_mday << " "
                << MONTHS[event_time.tm_mon];
        }
    }

    if (!timeStringStream.str().empty()) {
        time = new RenderableText(timeStringStream.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_RIGHT);
    }
    if (!dateStringStream.str().empty()) {
        date = new RenderableText(dateStringStream.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_LEFT);
    }

    if (entry.contains("summary") and entry["summary"] != nullptr) {
        description = new RenderableText(entry["summary"], 22, WHITE, FONTTYPE_REGULAR);
    }

    if (date != nullptr) {
        add_child(std::shared_ptr<RenderableText>(date), 0, 2);
    }
    if (daysRemaining != nullptr) {
        add_child(std::shared_ptr<RenderableText>(daysRemaining), CalendarEntry::w() - 5, 2);
    }
    if (time != nullptr) {
        add_child(std::shared_ptr<RenderableText>(time), CalendarEntry::w() - 5, 38);
    }
    if (description != nullptr) {
        add_child(std::shared_ptr<RenderableText>(description), 0, 22);
    }

    add_child(std::make_shared<Line>(GREY, CalendarEntry::w(), 0), 0, CalendarEntry::h());
}