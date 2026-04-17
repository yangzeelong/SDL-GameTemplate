#ifndef SCENE_DEMO_HPP_
#define SCENE_DEMO_HPP_

// ============================================
// Demo Scene
// Example scene demonstrating framework usage
// ============================================

#include "Scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>

class Game;

class SceneDemo : public Scene {
public:
    SceneDemo();
    ~SceneDemo() override;

public:
    // Initialize
    Error Init() override;
    Error DeInit() override;

    // Update logic
    void Update(float deltaTime) override;

    // Render
    void Render() override;

    // Handle input
    void HandleEvent(SDL_Event& event) override;

private:
    Game& m_game;
    TTF_Font* m_font;
    bool m_initialized;

    // Demo state - moving square
    float m_squareX;
    float m_squareY;
    float m_squareSpeed;
    int m_directionX;
    int m_directionY;
};

#endif
