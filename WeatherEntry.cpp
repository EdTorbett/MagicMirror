//
// Created by Ed on 12/12/2023.
//

#include "WeatherEntry.h"
#include <sstream>
#include <iostream>
#include <string>

WeatherEntry::WeatherEntry(const std::string& line_item) :
symbol(nullptr),
time(nullptr),
temperature(nullptr),
precipitation(nullptr),
wind(nullptr) {
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
    int wind_speed = std::stoi(item);
    if (wind_speed >= 15) {
        wind = new RenderableText(std::to_string(wind_speed) + "mph", 20, AMBER, FONTTYPE_MONO, ALIGN_CENTER);
    }
}

WeatherEntry::~WeatherEntry() {
    delete symbol;
    delete time;
    delete temperature;
    delete precipitation;
    delete wind;
}

void WeatherEntry::render(SDL_Renderer *renderer, const float brightness) {
    if (this->symbol != nullptr) {
        this->symbol->render(renderer, brightness);
    }
    if (this->time != nullptr) {
        this->time->render(renderer, brightness);
    }
    if (this->temperature != nullptr) {
        this->temperature->render(renderer, brightness);
    }
    if (this->precipitation != nullptr) {
        this->precipitation->render(renderer, brightness);
    }
    if (this->wind != nullptr) {
        this->wind->render(renderer, brightness);
    }
}

int WeatherEntry::x() const {
    return m_x;
}

int WeatherEntry::y() const {
    return m_y;
}

int WeatherEntry::w() const {
    return 90;
}

int WeatherEntry::h() const {
    return 150;
}

void WeatherEntry::set_pos(int x, int y) {
    m_x = x;
    m_y = y;

    if (this->time != nullptr) {
        this->time->set_pos(x + 45, y);
        y += this->time->h();
    }
    if (this->symbol != nullptr) {
        this->symbol->set_pos(x + 45, y);
        y += this->symbol->h();
    }
    if (this->temperature != nullptr) {
        this->temperature->set_pos(x + 45, y);
        y += this->temperature->h();
    }
    if (this->precipitation != nullptr) {
        this->precipitation->set_pos(x + 45, y);
        y += this->precipitation->h();
    }
    if (this->wind != nullptr) {
        this->wind->set_pos(x + 45, y);
    }
}