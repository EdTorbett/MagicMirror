//
// Created by Ed on 12/12/2023.
//

#include "WeatherEntry.h"
#include "RenderableText.h"
#include <sstream>
#include <iostream>
#include <string>

WeatherEntry::WeatherEntry(const std::string& line_item) {
    RenderableText *symbol = nullptr;
    RenderableText *time = nullptr;
    RenderableText *temperature = nullptr;
    RenderableText *precipitation = nullptr;
    RenderableText *wind = nullptr;

    std::stringstream ss(line_item);
    std::string item;
    getline (ss, item, ',');
    time = new RenderableText(item, 24, WHITE, FONTTYPE_MONO, ALIGN_CENTER);
    getline (ss, item, ',');
    std::string symbol_text = "\uF4BC";
    FontType font_type = FONTTYPE_SYMBOL;
    int symbol_size = 60;
    if (item == "cloudy") {
        symbol_text = "\uF409";
    } else if (item == "partlycloudy") {
        symbol_text = "\uF475";
    } else if (item == "partlycloudy-night") {
        symbol_text = "\uF40D";
    } else if (item == "sunny") {
        symbol_text = "\uF4B6";
    } else if (item == "rainy") {
        symbol_text = "\uF494";
    } else if (item == "clear-night") {
        symbol_text = "\uF468";
    } else if (item == "thunder") {
        symbol_text = "\uF4BC";
    } else {
        std::cout << "Unknown weather: " << item << std::endl;
        symbol_text = item;
        font_type = FONTTYPE_REGULAR;
        symbol_size = 30;
    }
    symbol = new RenderableText(symbol_text, symbol_size, WHITE, font_type, ALIGN_CENTER);
    getline (ss, item, ',');
    temperature = new RenderableText(item + "\u00B0C", 20, WHITE, FONTTYPE_MONO, ALIGN_CENTER);
    getline (ss, item, ',');
    if (item != "0.0") {
        precipitation = new RenderableText(item + "mm", 20, BLUE, FONTTYPE_MONO, ALIGN_CENTER);
    }
    getline (ss, item, ',');
    if (int wind_speed = std::stoi(item); wind_speed >= 15) {
        wind = new RenderableText(std::to_string(wind_speed) + "mph", 20, AMBER, FONTTYPE_MONO, ALIGN_CENTER);
    }

    int x = 45;
    int y = 0;

    add_child(std::shared_ptr<Renderable>(time), x, y);
    y += time->h();
    add_child(std::shared_ptr<Renderable>(symbol), x, y);
    y += symbol->h();
    add_child(std::shared_ptr<Renderable>(temperature), x, y);
    y += temperature->h();
    if (precipitation != nullptr) {
        add_child(std::shared_ptr<Renderable>(precipitation), x, y);
        y += precipitation->h();
    }
    if (wind != nullptr) {
        add_child(std::shared_ptr<Renderable>(wind), x, y);
    }
}

int WeatherEntry::w() const {
    return 90;
}

int WeatherEntry::h() const {
    return 150;
}
