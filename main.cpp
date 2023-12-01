#include <SDL.h>
#include <cstring>
#include <iostream>
#include <thread>

#include "RenderableText.h"

using namespace std::chrono_literals;
using std::clog;
using std::endl;

int main()
{
    text_init();

    auto init_re = SDL_VideoInit("KMSDRM");
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

    RenderableText hello_world("Hello, World!", 48.0);

    size_t frame_count = 0;
    int x_dir = 1;
    int y_dir = 1;
    while(true) {
        frame_count += 1;
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        hello_world.render(renderer);
        SDL_RenderPresent(renderer);
        std::this_thread::sleep_for(10ms);

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