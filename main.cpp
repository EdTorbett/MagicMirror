#include <SDL.h>
#include <iostream>
#include <fstream>
#include <array>

#include "RenderableText.h"
#include "HomeAssistant.h"
#include <cec.h>
#include <cecloader.h>

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
        std::cerr << "Unable to initialise the display!" << std::endl;
        exit(EXIT_FAILURE);
    }

    CEC::libcec_configuration cec_config;
    cec_config.Clear();
    cec_config.clientVersion       = CEC::LIBCEC_VERSION_CURRENT;
    cec_config.bActivateSource     = 0;
    cec_config.deviceTypes.Add(CEC::CEC_DEVICE_TYPE_PLAYBACK_DEVICE);
    strncpy(cec_config.strDeviceName, "Magic Mirror", LIBCEC_OSD_NAME_SIZE);
    CEC::ICECAdapter* cec_adapter = LibCecInitialise(&cec_config);
    cec_adapter->InitVideoStandalone();

    std::array<CEC::cec_adapter_descriptor,10> devices{};
    int8_t devices_found = cec_adapter->DetectAdapters(devices.data(), devices.size(), nullptr, true /*quickscan*/);
    if( devices_found <= 0)
    {
        std::cerr << "Could not automatically determine the cec adapter devices" << std::endl;
        UnloadLibCec(cec_adapter);
        return 1;
    }
    std::cout << "Opening the CEC device on port " << devices[0].strComName << std::endl;

    // Open a connection to the zeroth CEC device
    if( !cec_adapter->Open(devices[0].strComName) )
    {
        std::cerr << "Failed to open the CEC device on port " << devices[0].strComName << std::endl;
        UnloadLibCec(cec_adapter);
        return 1;
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

    int running = 1;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        homeAssistant.update(cec_adapter);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        homeAssistant.render(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cec_adapter->Close();
    UnloadLibCec(cec_adapter);
}