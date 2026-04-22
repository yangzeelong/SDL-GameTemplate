#ifndef SCENE_HPP_
#define SCENE_HPP_

// ============================================
// Scene Base Class
// All game scenes (Menu, Game, GameOver, etc.) inherit from this class
// ============================================

#include "Game/Error.hpp"
#include <SDL3/SDL.h>

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

public:
    // Initialize scene resources (fonts, textures, etc.)
    virtual Error Init() = 0;

    // Release scene resources
    virtual Error DeInit() = 0;

    // Update game logic
    // deltaTime: time since last frame in seconds
    virtual void Update(float deltaTime) = 0;

    // Render the scene
    virtual void Render() = 0;

    // Handle input events
    virtual void HandleEvent(SDL_Event& event) = 0;
};

#endif
