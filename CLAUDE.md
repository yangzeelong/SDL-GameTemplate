# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure (clangd + ninja)
cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1

# Build
cmake --build build

# Build with different generator
cmake -S . -B build
cmake --build build
```

Output executable: `SDL-GameTemplate.exe` in project root.

## Architecture

### Scene System
All game scenes inherit from `Scene` base class. Game manages scene transitions via `ChangeScene(std::unique_ptr<Scene>)`. The scene lifecycle: `Init()` -> `Update(deltaTime)` / `Render()` / `HandleEvent(event)` -> `DeInit()`.

Key files: `src/Game/Scene.hpp`, `src/Game/Game.cpp` (scene management)

### Config Module (Split Architecture)
- **Data**: `src/Game/Settings.hpp/cpp` - Runtime configuration values (WindowWidth, FPS, ShowFPS, etc.)
- **Parser**: `src/Base/Config/` - Config file loading infrastructure
  - `ConfigParser.hpp` - Singleton manager with `CONFIG_INIT`, `CONFIG_GET_VALUE_OR_DEFAULT` macros
  - `ParserBase.hpp` - Abstract interface
  - `ParserIni.hpp/cpp` - INI file implementation

Settings are loaded from `assets/config.ini` via `Settings::Init()` at game startup.

### Game Singleton
`Game::GetInstance()` manages SDL initialization, main loop, window/renderer, and scene transitions. Private constructor, deleted copy/assignment.

## Code Patterns

### Adding a new Scene
1. Inherit `Scene` class
2. Implement: `Init()`, `DeInit()`, `Update(float)`, `Render()`, `HandleEvent(SDL_Event&)`
3. In `main.cpp`: `game.ChangeScene(std::make_unique<MyScene>())`

### Configuration Access
```cpp
CONFIG_GET_VALUE(section, key, outValue);           // Get value, error if missing
CONFIG_GET_VALUE_OR_DEFAULT(section, key, default); // Use default if missing
```

Config sections in `assets/config.ini`: `Window`, `FPS`, etc.

### Frame Timing
Game loop uses nanosecond precision via `SDL_GetTicksNS()`. Delta time passed to `Update(float)` in seconds.