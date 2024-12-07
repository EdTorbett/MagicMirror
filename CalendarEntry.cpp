//
// Created by Edward Torbett on 10/12/2023.
//

#include "CalendarEntry.h"
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

CalendarEntry::CalendarEntry(const nlohmann::json &entry, const std::chrono::system_clock::time_point &today) :
date(nullptr),
description(nullptr),
time(nullptr),
daysRemaining(nullptr) {
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

        this->daysRemaining = new RenderableText(daysToEvent.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_RIGHT);
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
        this->time = new RenderableText(timeStringStream.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_RIGHT);
    }
    if (!dateStringStream.str().empty()) {
        this->date = new RenderableText(dateStringStream.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_LEFT);
    }

    if (entry.contains("summary") and entry["summary"] != nullptr) {
        this->description = new RenderableText(entry["summary"], 22, WHITE, FONTTYPE_REGULAR);
    }
}

CalendarEntry::~CalendarEntry() {
    delete date;
    delete time;
    delete description;
    delete daysRemaining;
}

void CalendarEntry::render(SDL_Renderer *renderer) {
    if (this->date != nullptr) {
        this->date->render(renderer);
    }
    if (this->time != nullptr) {
        this->time->render(renderer);
    }
    if (this->description != nullptr) {
        this->description->render(renderer);
    }
    if (this->daysRemaining != nullptr) {
        this->daysRemaining->render(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_RenderDrawLine(renderer, this->y() + this->h(), 1080 - this->x(), this->y() + this->h(), 1080 - (this->x() + this->w()));
}

int CalendarEntry::x() const {
    return this->date->x();
}

int CalendarEntry::y() const {
    return this->date->y() - 2;
}

int CalendarEntry::w() const {
    return 440;
}

int CalendarEntry::h() const {
    return 60;
}

void CalendarEntry::set_pos(int x, int y) {
    if (this->date != nullptr) {
        this->date->set_pos(x, y + 2);
    }
    if (this->daysRemaining != nullptr) {
        this->daysRemaining->set_pos(x + (this->w() - 5), y + 2);
    }
    if (this->time != nullptr) {
        this->time->set_pos(x + (this->w() - 5), y + 38);
    }
    if (this->description != nullptr) {
        this->description->set_pos(x, y + 22);
    }
}