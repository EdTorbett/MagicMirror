//
// Created by Ed on 12/12/2023.
//

#ifndef WEATHERENTRY_H
#define WEATHERENTRY_H


#include "Renderable.h"
#include "RenderableText.h"
#include <nlohmann/json.hpp>


class WeatherEntry : Renderable {
public:
    explicit WeatherEntry(const std::string &line_item);
    ~WeatherEntry() override;
    void render(SDL_Renderer *renderer, float brightness) override;
    int x() const override;
    int y() const override;
    int w() const override;
    int h() const override;
    void set_pos(int x, int y) override;
private:
    RenderableText *symbol;
    RenderableText *time;
    RenderableText *temperature;
    RenderableText *precipitation;
    RenderableText *wind;
    int m_x = 0;
    int m_y = 0;
};



#endif //WEATHERENTRY_H
