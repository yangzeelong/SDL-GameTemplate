#ifndef SCENE_DEMO_HPP_
#define SCENE_DEMO_HPP_

// ============================================
// Demo Scene
// Example scene demonstrating framework usage
// ============================================

#include "Scene.hpp"
#include <SDL3/SDL.h>

class Game;

class SceneDemo : public Scene {
public:
    SceneDemo();
    ~SceneDemo() override;

public:
    Error Init() override;
    Error DeInit() override;
    void Update(float deltaTime) override;
    void Render() override;
    void HandleEvent(SDL_Event& event) override;

private:
    Game& m_game;
    bool m_initialized;

    // Bouncing square
    float m_squareX;
    float m_squareY;
    float m_squareSpeed;
    int m_directionX;
    int m_directionY;

    // Cube rotation
    float m_rotation;
    float m_time;
};

#endif
