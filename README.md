# SDL Game Template

A lightweight game framework template based on SDL3 for rapid 2D game demo development.

## Features

- **SDL3 Core**: Video, Renderer, Window management
- **SDL3_image**: Image loading (PNG, JPG, BMP, etc.)
- **SDL3_mixer**: Audio playback (WAV, MP3, OGG, etc.)
- **SDL3_ttf**: TrueType font rendering
- **Scene System**: Easy scene management and transitions
- **60 FPS**: Fixed timestep game loop with frame rate control
- **Config System**: INI-based configuration with runtime parameter access

## Required DLLs

The following DLLs are required in the same directory as the executable:

| DLL | Purpose |
|-----|---------|
| `SDL3.dll` | SDL3 core library |
| `SDL3_image.dll` | Image loading (PNG, JPG, BMP, GIF, etc.) |
| `SDL3_mixer.dll` | Audio playback (WAV, MP3, OGG, FLAC, etc.) |
| `SDL3_ttf.dll` | TrueType font rendering |

These are automatically copied to the output directory during build.

## Directory Structure

```
SDL-GameTemplate/
├── CMakeLists.txt          # CMake build configuration
├── CLAUDE.md               # Claude Code guidance
├── README.md               # This file
├── assets/
│   ├── config.ini          # Runtime configuration
│   └── font/               # Bitmap fonts for FPS display
└── src/
    ├── main.cpp            # Program entry point
    ├── Base/
    │   ├── Config/         # Configuration module
    │   │   ├── ConfigParser.hpp   # Config manager + macros
    │   │   ├── ParserBase.hpp     # Parser interface
    │   │   └── ParserIni.hpp/cpp  # INI file parser
    │   └── File/            # File utilities
    │       └── File.hpp/cpp       # Directory creation, path utils
    └── Game/
        ├── Error.hpp       # Error codes (SUCCESS, ERR, ERROR_INIT)
        ├── Game.hpp/cpp    # Core framework (SDL init, main loop)
        ├── Scene.hpp       # Scene base class
        ├── SceneDemo.hpp/cpp # Demo scene
        └── Settings.hpp/cpp  # Runtime configuration values
```

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .

# clangd + ninja (recommended)
cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build build
```

## Configuration

Edit `assets/config.ini`:

```ini
[Window]
width = 800
height = 600
title = My Game

[FPS]
target = 60
show = true    ; Display FPS counter in top-left corner
```

## Quick Start

### Run

```bash
./SDL-GameTemplate.exe
```

### Add a New Scene

**1. Create header** (`src/Game/SceneMyGame.hpp`):

```cpp
#include "Scene.hpp"
#include <SDL3/SDL.h>

class SceneMyGame : public Scene {
public:
    Error Init() override;
    Error DeInit() override;
    void Update(float deltaTime) override;
    void Render() override;
    void HandleEvent(SDL_Event& event) override;
};
```

**2. Implement in** `src/Game/SceneMyGame.cpp`.

**3. Switch to it** in `main.cpp`:

```cpp
game.ChangeScene(std::make_unique<SceneMyGame>());
```

## Main Loop

```
Each frame (~16.67ms @ 60FPS):
1. HandleEvent()   - Process all pending SDL events
2. Update(dt)      - Update game logic (dt = seconds)
3. Render()        - Clear + render scene + present
4. Delay           - Frame rate control
```

## Scene Lifecycle

```
SceneA::Update() or HandleEvent()
    ↓
game.ChangeScene(std::make_unique<SceneB>())
    ↓
SceneA::DeInit() → SceneB::Init()
    ↓
Next frame renders SceneB
```

## Code Patterns

### Input Handling

```cpp
void MyScene::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_ESCAPE: game.Quit(); break;
            case SDLK_UP: /* move up */ break;
        }
    }
}

// Polling (for held keys)
void MyScene::Update(float) {
    const bool* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W]) { /* W held */ }
}
```

### Rendering

```cpp
// Filled rectangle
SDL_FRect rect = {x, y, w, h};
SDL_SetRenderDrawColor(renderer, r, g, b, a);
SDL_RenderFillRect(renderer, &rect);

// Text
SDL_Surface* surf = TTF_RenderText_Blended(font, text, strlen, color);
SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
SDL_RenderTexture(renderer, tex, nullptr, &dst);
SDL_DestroyTexture(tex);
SDL_DestroySurface(surf);

// Image
SDL_Surface* surf = SDL_LoadPNG("path/to/img.png");
SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
SDL_RenderTexture(renderer, tex, nullptr, &dst);
SDL_DestroyTexture(tex);
SDL_DestroySurface(surf);
```

### Audio (SDL3_mixer)

```cpp
Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
Mix_Music* music = Mix_LoadMUS("music.mp3");
Mix_PlayMusic(music, -1);  // loop forever
Mix_PauseMusic();
Mix_HaltMusic();
Mix_FreeMusic(music);
```

## Learning Resources

- [SDL3 Official Documentation](https://wiki.libsdl.org/)
- [SDL3_image](https://github.com/libsdl-org/SDL_image)
- [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)

## License

Free to use for any purpose.