//
// Created by Ed on 12/12/2023.
//

#include "WeatherEntry.h"
#include <sstream>
#include <iostream>

WeatherEntry::WeatherEntry(const std::string& line_item) :
symbol(nullptr),
time(nullptr),
temperature(nullptr),
precipitation(nullptr) {
    std::stringstream ss(line_item);
    std::string item;
    getline (ss, item, ',');
    time = new RenderableText(item, 24, WHITE, FONTTYPE_MONO, ALIGN_CENTER);
    getline (ss, item, ',');
    std::string symbol_text = "\uF4BC";
    int symbol_size = 60;
    if (item == "cloudy") {
        symbol_text = "\uF409";
    } else if (item == "partlycloudy") {
        symbol_text = "\uF475";
    } else if (item == "sunny") {
        symbol_text = "\uF4B6";
    } else if (item == "rainy") {
        symbol_text = "\uF494";
        symbol_size = 80;
    } else {
        std::cout << "Unknown weather: " << item << std::endl;
    }
    symbol = new RenderableText(symbol_text, symbol_size, WHITE, FONTTYPE_SYMBOL, ALIGN_RIGHT);
    getline (ss, item, ',');
    temperature = new RenderableText(item + "\u00B0C", 20, WHITE, FONTTYPE_MONO, ALIGN_LEFT);
    getline (ss, item, ',');
    if (item != "0.0") {
        precipitation = new RenderableText(item + "mm", 20, BLUE, FONTTYPE_MONO, ALIGN_LEFT);
    }
}

WeatherEntry::~WeatherEntry() {
    delete symbol;
    delete time;
    delete temperature;
    delete precipitation;
}

void WeatherEntry::render(SDL_Renderer *renderer) {
    if (this->symbol != nullptr) {
        this->symbol->render(renderer);
    }
    if (this->time != nullptr) {
        this->time->render(renderer);
    }
    if (this->temperature != nullptr) {
        this->temperature->render(renderer);
    }
    if (this->precipitation != nullptr) {
        this->precipitation->render(renderer);
    }
}

int WeatherEntry::x() const {
    return m_x;
}

int WeatherEntry::y() const {
    return m_y;
}

int WeatherEntry::w() const {
    return 150;
}

int WeatherEntry::h() const {
    return 88;
}

void WeatherEntry::set_pos(int x, int y) {
    m_x = x;
    m_y = y;

    if (this->time != nullptr) {
        this->time->set_pos(x + 75, y);
    }
    if (this->symbol != nullptr) {
        this->symbol->set_pos(x + 70, y + 60 - this->symbol->h() / 2);
    }
    if (this->temperature != nullptr) {
        this->temperature->set_pos(x + 80, y + 35);
    }
    if (this->precipitation != nullptr) {
        this->precipitation->set_pos(x + 80, y + 60);
    }
}