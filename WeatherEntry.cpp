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
    time = new RenderableText(item, 16, WHITE, FONTTYPE_MONO);
    getline (ss, item, ',');
    if (item == "cloudy") {
        symbol = new RenderableText("\uF409", 40, WHITE, FONTTYPE_SYMBOL);
    } else if (item == "partlycloudy") {
        symbol = new RenderableText("\uF475", 40, WHITE, FONTTYPE_SYMBOL);
    } else if (item == "sunny") {
        symbol = new RenderableText("\uF4B6", 40, WHITE, FONTTYPE_SYMBOL);
    } else if (item == "rainy") {
        symbol = new RenderableText("\uF494", 40, WHITE, FONTTYPE_SYMBOL);
    } else {
        std::cout << "Unknown weather: " << item << std::endl;
        symbol = new RenderableText("\uF4BC", 40, WHITE, FONTTYPE_SYMBOL);
    }

    getline (ss, item, ',');
    temperature = new RenderableText(item + "\u00B0C", 16, WHITE, FONTTYPE_MONO);
    getline (ss, item, ',');
    if (item != "0.0") {
        precipitation = new RenderableText(item + "mm", 16, BLUE, FONTTYPE_MONO);
    }
}

WeatherEntry::~WeatherEntry() {
    delete symbol;
    delete time;
    delete temperature;
    delete precipitation;
}

void WeatherEntry::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x20, 0x60, 0xFF);
    SDL_Rect r = {this->x(), this->y(), 150, 40};
    SDL_RenderFillRect(renderer, &r);

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
    return this->symbol->x();
}

int WeatherEntry::y() const {
    return this->symbol->y();
}

int WeatherEntry::w() const {
    return 150;
}

int WeatherEntry::h() const {
    return 88;
}

void WeatherEntry::set_pos(int x, int y) {
    if (this->symbol != nullptr) {
        this->symbol->set_pos(x, y);
    }
    if (this->time != nullptr) {
        this->time->set_pos(x, y + 40);
    }
    if (this->temperature != nullptr) {
        this->temperature->set_pos(x, y + 56);
    }
    if (this->precipitation != nullptr) {
        this->precipitation->set_pos(x, y + 72);
    }
}