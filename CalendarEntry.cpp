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
location(nullptr),
daysRemaining(nullptr) {
    time_t event_start_time_t;
    tm event_time({0});
    if (entry.contains("start")) {
        std::string startTime;
        std::ostringstream oss;
        const nlohmann::json &start = entry[START_KEY];
        if (start.contains(DATETIME_KEY)) {
            std::istringstream ss(std::string(start[DATETIME_KEY]).substr(0, 10));
            ss >> std::get_time(&event_time, "%Y-%m-%d");
            startTime = "Starts at " + std::string(start[DATETIME_KEY]).substr(11, 5);
        } else if (start.contains(DATE_KEY)) {
            std::string d = start[DATE_KEY];
            std::istringstream ss(d);
            ss >> std::get_time(&event_time, "%Y-%m-%d");
        }
        event_start_time_t = mktime(&event_time);
        event_time = *localtime(&event_start_time_t);

        oss.str("");
        oss.clear();
        oss << DAYS_OF_WEEK[event_time.tm_wday] << " " << std::setw(2) << event_time.tm_mday << " " << MONTHS[event_time.tm_mon];
        this->date = new RenderableText(oss.str(), 16, YELLOW, FONTTYPE_MONO);

        auto event_date = std::chrono::system_clock::from_time_t(event_start_time_t);
        auto days_till_event = floor<std::chrono::hours>(event_date - today).count() / 24;

        oss.str("");
        oss.clear();
        if (days_till_event == 0) {
            oss << "Today";
        } else if (days_till_event == 1) {
            oss << "Tomorrow";
        } else if (days_till_event == -1) {
            oss << "Started yesterday";
        } else if (days_till_event < 0) {
            oss << "Started " << -days_till_event << " days ago";
        } else {
            oss << days_till_event << " days time";
        }

        this->daysRemaining = new RenderableText(oss.str(), 16, YELLOW, FONTTYPE_MONO, ALIGN_RIGHT);

        if (!startTime.empty()) {
            this->time = new RenderableText(startTime, 16, YELLOW, FONTTYPE_MONO);
        }
    }

    if (entry.contains(END_KEY) && entry[END_KEY].contains(DATE_KEY)) {
        std::string d = entry[END_KEY][DATE_KEY];
        std::istringstream ss(d);
        ss >> std::get_time(&event_time, "%Y-%m-%d");
        time_t event_end_time_t = mktime(&event_time);
        std::ostringstream oss;
        if (event_end_time_t == event_start_time_t + 86400) {
            oss << "All day";
        } else {
            event_end_time_t -= 1;
            event_time = *localtime(&event_end_time_t);
            oss << "Until " << DAYS_OF_WEEK[event_time.tm_wday] << " " << std::setw(2) << event_time.tm_mday << " "
                << MONTHS[event_time.tm_mon];
        }
        this->time = new RenderableText(oss.str(), 16, YELLOW, FONTTYPE_MONO);
    }

    if (entry.contains("summary") and entry["summary"] != nullptr) {
        this->description = new RenderableText(entry["summary"], 22, WHITE, FONTTYPE_REGULAR);
    }
    if (entry.contains("location") and entry["location"] != nullptr) {
        this->location = new RenderableText(entry["location"], 16, GREEN, FONTTYPE_MONO, ALIGN_RIGHT);
    }
}

CalendarEntry::~CalendarEntry() {
    delete date;
    delete time;
    delete description;
    delete location;
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
    if (this->location != nullptr) {
        this->location->render(renderer);
    }
    if (this->daysRemaining != nullptr) {
        this->daysRemaining->render(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_RenderDrawLine(renderer, this->x(), this->y() + this->h(), this->x() + this->w(), this->y() + this->h());
}

int CalendarEntry::x() const {
    return this->date->x() - 10;
}

int CalendarEntry::y() const {
    return this->date->y() - 4;
}

int CalendarEntry::w() const {
    return 450;
}

int CalendarEntry::h() const {
    return 80;
}

void CalendarEntry::set_pos(int x, int y) {
    if (this->date != nullptr) {
        this->date->set_pos(x + 10, y + 2);
    }
    if (this->daysRemaining != nullptr) {
        this->daysRemaining->set_pos(x + (this->w() - 5), y + 2);
    }
    if (this->time != nullptr) {
        this->time->set_pos(x + 10, y + 53);
    }
    if (this->description != nullptr) {
        this->description->set_pos(x + 10, y + 22);
    }
    if (this->location != nullptr) {
        this->location->set_pos(x + (this->w() - 5), y + 53);
    }

}