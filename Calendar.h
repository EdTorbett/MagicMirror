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


class Calendar final : public Renderable {
public:
    Calendar();
    void fetch(RestClient::Connection *connection);
    [[nodiscard]] int w() const override { return 980; };
    [[nodiscard]] int h() const override { return 300; };
};



#endif //CALENDAR_H
