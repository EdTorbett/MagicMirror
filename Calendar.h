//
// Created by Ed on 01/01/2024.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <connection.h>
#include <memory>
#include <vector>

#include "CalendarEntry.h"
#include "Renderable.h"


class Calendar : Renderable {
public:
    Calendar();
    ~Calendar() override;
    void fetch(RestClient::Connection *connection);
    void render(SDL_Renderer *renderer, float brightness) override;
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
    [[nodiscard]] int w() const override;
    [[nodiscard]] int h() const override;
    void set_pos(int x, int y) override;
private:
    int m_x = 0;
    int m_y = 0;
    std::vector<std::shared_ptr<CalendarEntry>> m_entries;
    void update_entry_positions();
};



#endif //CALENDAR_H
