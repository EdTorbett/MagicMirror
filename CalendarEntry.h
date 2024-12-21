//
// Created by Edward Torbett on 10/12/2023.
//

#ifndef MAGICMIRROR_CALENDARENTRY_H
#define MAGICMIRROR_CALENDARENTRY_H

#include "Renderable.h"
#include <nlohmann/json.hpp>


class CalendarEntry final : public Renderable {
public:
    CalendarEntry(const nlohmann::json &entry, const std::chrono::system_clock::time_point &today);
    [[nodiscard]] int w() const override { return 440; }
    [[nodiscard]] int h() const override { return 60; };
};


#endif //MAGICMIRROR_CALENDARENTRY_H
