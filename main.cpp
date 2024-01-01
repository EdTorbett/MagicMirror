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

    SDL_Event event;

    homeAssistant.fetch();

    int running = 1;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        homeAssistant.render(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}