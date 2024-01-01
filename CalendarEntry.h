//
// Created by Edward Torbett on 10/12/2023.
//

#ifndef MAGICMIRROR_CALENDARENTRY_H
#define MAGICMIRROR_CALENDARENTRY_H

#include "Renderable.h"
#include "RenderableText.h"
#include <nlohmann/json.hpp>
#include <chrono>


class CalendarEntry : Renderable {
public:
    CalendarEntry(const nlohmann::json &entry, const std::chrono::system_clock::time_point &today);
    ~CalendarEntry() override;
    void render(SDL_Renderer *renderer) override;
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
    [[nodiscard]] int w() const override;
    [[nodiscard]] int h() const override;
    void set_pos(int x, int y) override;
private:
    RenderableText *date;
    RenderableText *time;
    RenderableText *description;
    RenderableText *location;
    RenderableText *daysRemaining;
};


#endif //MAGICMIRROR_CALENDARENTRY_H
