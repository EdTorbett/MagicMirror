//
// Created by Ed on 01/01/2024.
//

#ifndef FORECAST_H
#define FORECAST_H
#include <connection.h>

#include "Renderable.h"

class Forecast final : public Renderable {
public:
    Forecast();
    void fetch(RestClient::Connection *connection);
    [[nodiscard]] int w() const override;
    [[nodiscard]] int h() const override;
};



#endif //FORECAST_H
