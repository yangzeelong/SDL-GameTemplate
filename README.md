# SDL Game Template

A lightweight game framework template based on SDL3 for rapid 2D game demo development.

## Features

- **SDL3 Core**: Video, Renderer, Window management
- **SDL3_image**: Image loading (PNG, JPG, BMP, etc.)
- **SDL3_mixer**: Audio playback (WAV, MP3, OGG, etc.)
- **SDL3_ttf**: TrueType font rendering
- **Scene System**: Easy scene management and transitions
- **60 FPS**: Fixed timestep game loop with frame rate control

## Required DLLs

The following DLLs are required in the same directory as the executable:

| DLL | Purpose |
|-----|---------|
| `SDL3.dll` | SDL3 core library |
| `SDL3_image.dll` | Image loading (PNG, JPG, BMP, GIF, etc.) |
| `SDL3_mixer.dll` | Audio playback (WAV, MP3, OGG, FLAC, etc.) |
| `SDL3_ttf.dll` | TrueType font rendering |

These are automatically copied to the output directory during build. The SDL3 libraries bundle all necessary codec support internally.

## Directory Structure

```
SDL-GameTemplate/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
└── src/
    ├── main.cpp            # Program entry point
    └── Game/
        ├── Config.hpp      # Configuration (window size, colors, etc.)
        ├── Error.hpp       # Error codes
        ├── Game.hpp/cpp    # Core framework (SDL init, main loop, scene mgmt)
        ├── Scene.hpp       # Scene base class
        └── SceneDemo.hpp/cpp  # Demo scene showing framework usage
```

## Quick Start

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

```bash
./SDL-GameTemplate.exe
```

### Modify Configuration

Edit `src/Game/Config.hpp`:

```cpp
constexpr int kWindowWidth = 800;                    // Window width
constexpr int kWindowHeight = 600;                    // Window height
constexpr const char* kWindowTitle = "My Game";      // Window title
constexpr int kFPS = 60;                              // Target frame rate

// Grid settings for tile-based games
constexpr int kGridCols = 20;
constexpr int kGridRows = 15;
constexpr int kCellSize = 32;
```

## How to Add a New Scene

### 1. Create Scene Header

```cpp
// src/Game/SceneMyGame.hpp
#ifndef SCENE_MYGAME_HPP_
#define SCENE_MYGAME_HPP_

#include "Scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>

class Game;

class SceneMyGame : public Scene {
public:
    SceneMyGame();
    ~SceneMyGame() override;

public:
    Error Init() override;
    Error DeInit() override;
    void Update(float deltaTime) override;
    void Render() override;
    void HandleEvent(SDL_Event& event) override;

private:
    Game& m_game;
    TTF_Font* m_font;
    // Add your game data here...
};

#endif
```

### 2. Implement Scene

```cpp
// src/Game/SceneMyGame.cpp
#include "SceneMyGame.hpp"
#include "Game.hpp"
#include "Config.hpp"

SceneMyGame::SceneMyGame()
    : m_game(Game::GetInstance())
    , m_font(nullptr) {
}

Error SceneMyGame::Init() {
    // Initialize font, load textures, etc.
    return Error::SUCCESS;
}

Error SceneMyGame::DeInit() {
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
    return Error::SUCCESS;
}

void SceneMyGame::Update(float deltaTime) {
    // Update game logic
}

void SceneMyGame::Render() {
    SDL_Renderer* renderer = m_game.GetRenderer();
    // Render game画面
}

void SceneMyGame::HandleEvent(SDL_Event& event) {
    // Handle input events
}
```

### 3. Switch Scene in main.cpp

```cpp
// src/main.cpp
#include "Game/Game.hpp"
#include "Game/SceneMyGame.hpp"

int main(int, char**) {
    auto& game = Game::GetInstance();
    game.Init();

    // Switch to your scene
    game.ChangeScene(std::make_unique<SceneMyGame>());

    game.Run();
    game.DeInit();
    return 0;
}
```

### 4. Build

```bash
cmake --build .
# CMake auto-includes all .cpp files in src/Game/
```

## Framework Reference

### Game Class

Core manager class (singleton pattern):

| Method | Description |
|--------|-------------|
| `GetInstance()` | Get Game instance |
| `Init()` | Initialize SDL window and renderer |
| `DeInit()` | Release resources |
| `Run()` | Run main loop |
| `ChangeScene(scene)` | Switch to another scene |
| `Exit()` | Exit the game |
| `GetRenderer()` | Get SDL renderer |

### Scene Class

Base class for all scenes:

| Method | Description |
|--------|-------------|
| `Init()` | Initialize scene resources |
| `DeInit()` | Release scene resources |
| `Update(dt)` | Update game logic (dt = seconds) |
| `Render()` | Render the scene |
| `HandleEvent(event)` | Handle input events |

### Scene Transition Flow

```
SceneA::Update() or HandleEvent()
    ↓
game.ChangeScene(std::make_unique<SceneB>());
    ↓
SceneA::DeInit() is called
SceneB::Init() is called
    ↓
Next frame renders SceneB
```

### Main Loop

```
Each frame (~16.67ms @ 60FPS):
1. HandleEvent()   - Process all pending events
2. Update(dt)       - Update game logic
3. Render()         - Clear + render scene + present
4. Delay            - Frame rate control
```

## Input Handling

### Keyboard Events

```cpp
void MyScene::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_UP:      /* Up arrow */ break;
            case SDLK_DOWN:    /* Down arrow */ break;
            case SDLK_LEFT:    /* Left arrow */ break;
            case SDLK_RIGHT:   /* Right arrow */ break;
            case SDLK_ESCAPE:  /* Escape */ break;
            case SDLK_SPACE:   /* Space */ break;
            case SDLK_RETURN:  /* Enter */ break;
            case SDLK_w:       /* W key */ break;
            case SDLK_a:       /* A key */ break;
            case SDLK_s:       /* S key */ break;
            case SDLK_d:       /* D key */ break;
        }
    }
}
```

### Keyboard State (Polling)

```cpp
void MyScene::Update(float deltaTime) {
    const bool* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W]) { /* W is pressed */ }
    if (keys[SDL_SCANCODE_UP]) { /* Up is pressed */ }
}
```

## Rendering Examples

### Filled Rectangle

```cpp
SDL_FRect rect = {x, y, width, height};
SDL_SetRenderDrawColor(renderer, r, g, b, a);
SDL_RenderFillRect(renderer, &rect);
```

### Rectangle Outline

```cpp
SDL_FRect rect = {x, y, width, height};
SDL_SetRenderDrawColor(renderer, r, g, b, a);
SDL_RenderRect(renderer, &rect);
```

### Line

```cpp
SDL_SetRenderDrawColor(renderer, r, g, b, a);
SDL_RenderLine(renderer, x1, y1, x2, y2);
```

### Text

```cpp
const char* text = "Hello World";
SDL_Color white = {255, 255, 255, 255};
SDL_Surface* surf = TTF_RenderText_Blended(m_font, text, SDL_strlen(text), white);
SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
SDL_FRect dst = {x, y, tw, th};
SDL_RenderTexture(renderer, tex, nullptr, &dst);
SDL_DestroyTexture(tex);
SDL_DestroySurface(surf);
```

### Image (SDL3_image)

```cpp
SDL_Surface* surf = SDL_LoadBMP("path/to/image.bmp");
// or SDL_LoadJPG("path/to/image.jpg");
// or SDL_LoadPNG("path/to/image.png");
SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
SDL_DestroySurface(surf);
SDL_RenderTexture(renderer, tex, nullptr, &dst);
SDL_DestroyTexture(tex);
```

## Audio (SDL3_mixer)

```cpp
#include <SDL3_mixer/SDL_mixer.h>

// Initialize (call once before playing)
Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);

// Load sound effect
Mix_Chunk* sound = Mix_LoadWAV("effect.wav");

// Play sound effect
Mix_PlayChannel(-1, sound, 0);  // -1 = first available channel

// Load music
Mix_Music* music = Mix_LoadMUS("music.mp3");

// Play music
Mix_PlayMusic(music, -1);  // -1 = loop forever

// Pause/Resume
Mix_PauseMusic();
Mix_ResumeMusic();

// Stop
Mix_HaltMusic();

// Cleanup
Mix_FreeChunk(sound);
Mix_FreeMusic(music);
```

## Common SDL3 APIs

### Time

```cpp
uint32_t timeMs = SDL_GetTicks();    // Milliseconds since SDL_Init
float timeS = timeMs / 1000.0f;       // Convert to seconds
```

### Delay

```cpp
SDL_Delay(16);  // Delay 16 milliseconds
```

### Random Numbers

```cpp
#include <random>
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 100);
int randomNum = dis(gen);
```

## Adding Resources

Place resource files in an `assets/` directory:

```
SDL-GameTemplate/
├── assets/
│   ├── images/
│   │   └── player.png
│   ├── audio/
│   │   ├── music.mp3
│   │   └── effect.wav
│   └── fonts/
│       └── custom.ttf
```

Load them with:

```cpp
SDL_Surface* surf = SDL_LoadPNG("assets/images/player.png");
Mix_Music* music = Mix_LoadMUS("assets/audio/music.mp3");
TTF_Font* font = TTF_OpenFont("assets/fonts/custom.ttf", 24);
```

## Learning Resources

- [SDL3 Official Documentation](https://wiki.libsdl.org/)
- [SDL3_image Documentation](https://github.com/libsdl-org/SDL_image)
- [SDL3_mixer Documentation](https://github.com/libsdl-org/SDL_mixer)
- [SDL3_ttf Documentation](https://github.com/libsdl-org/SDL_ttf)

## License

This template is free to use for any purpose.
