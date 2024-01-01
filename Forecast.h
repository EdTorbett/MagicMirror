//
// Created by Ed on 01/01/2024.
//

#ifndef FORECAST_H
#define FORECAST_H
#include <connection.h>
#include <memory>
#include <vector>

#include "Renderable.h"
#include "WeatherEntry.h"

class Forecast : Renderable {
public:
    Forecast();
    ~Forecast() override;
    void fetch(RestClient::Connection *connection);
    void render(SDL_Renderer *renderer) override;
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
    [[nodiscard]] int w() const override;
    [[nodiscard]] int h() const override;
    void set_pos(int x, int y) override;
private:
    int m_x = 0;
    int m_y = 0;
    std::vector<std::shared_ptr<WeatherEntry>> m_entries;
    void update_entry_positions();
};



#endif //FORECAST_H
