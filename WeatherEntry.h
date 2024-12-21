//
// Created by Ed on 12/12/2023.
//

#ifndef WEATHERENTRY_H
#define WEATHERENTRY_H


#include "Renderable.h"
#include <nlohmann/json.hpp>


class WeatherEntry : public Renderable {
public:
    explicit WeatherEntry(const std::string &line_item);
    [[nodiscard]] int w() const override;
    [[nodiscard]] int h() const override;
};



#endif //WEATHERENTRY_H
