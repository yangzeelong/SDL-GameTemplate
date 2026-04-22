#include "Settings.hpp"
#include "Config/ConfigParser.hpp"
#include <SDL3/SDL_log.h>

// clang-format off
// Default config values
int Settings::WindowWidth = 800;
int Settings::WindowHeight = 600;
const char* Settings::WindowTitle = "SDL Game Template";

int Settings::FPS = 60;
float Settings::FrameDelay = 1000.0f / Settings::FPS;
bool Settings::ShowFPS = false;
// clang-format on

void Settings::Init() {
    if (!CONFIG_IS_INIT()) {
        CONFIG_INIT("assets/config.ini");
    }

    WindowWidth = CONFIG_GET_VALUE_OR_DEFAULT("Window", "width", 800);
    WindowHeight = CONFIG_GET_VALUE_OR_DEFAULT("Window", "height", 600);

    // Copy title to static buffer
    static char titleBuf[256];
    std::string title = CONFIG_GET_VALUE_OR_DEFAULT("Window", "title", std::string("SDL Game Template"));
    SDL_strlcpy(titleBuf, title.c_str(), sizeof(titleBuf));
    WindowTitle = titleBuf;

    FPS = CONFIG_GET_VALUE_OR_DEFAULT("FPS", "target", 60);
    FrameDelay = 1000.0f / FPS;
    ShowFPS = CONFIG_GET_VALUE_OR_DEFAULT("FPS", "show", false);

    SDL_Log("Settings: %dx%d, FPS=%d, ShowFPS=%s", WindowWidth, WindowHeight, FPS, ShowFPS ? "true" : "false");
}
