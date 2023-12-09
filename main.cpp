#include <SDL.h>
#include <iostream>
#include <fstream>

#include "RenderableText.h"
#include "HomeAssistant.h"

using namespace std::chrono_literals;
using std::clog;
using std::endl;

#ifdef TARGET_OS_OSX
#define SDL_DRIVER "cocoa"
#else
#define SDL_DRIVER "KMSDRM"
#endif

int main()
{
    text_init();

    auto init_re = SDL_VideoInit(SDL_DRIVER);
    if (init_re != 0) {
        exit(EXIT_FAILURE);
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_SHOWN, &window, &renderer);
    if (nullptr == window)
    {
        std::cerr << "SDL_CreateWindow(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }

    HomeAssistant homeAssistant;

    RenderableText hello_world("Hello, World!", 48.0);

    SDL_Event event;

    homeAssistant.getCalendar();

    size_t frame_count = 0;
    int x_dir = 1;
    int y_dir = 1;
    int running = 1;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        frame_count += 1;
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        hello_world.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);

        int x = hello_world.x();
        int y = hello_world.y();

        if (x + x_dir + hello_world.w() >= 1920) {
            x_dir = -1;
        } else if (x == 0) {
            x_dir = 1;
        }
        if (y + y_dir + hello_world.h() >= 1080) {
            y_dir = -1;
        } else if (y == 0) {
            y_dir = 1;
        }

        x += x_dir;
        y += y_dir;

        hello_world.set_pos(x, y);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}