#ifndef GAME_HPP_
#define GAME_HPP_

// ============================================
// Game Framework
// Core game framework managing SDL initialization,
// main loop, and scene transitions
// ============================================

#include "Error.hpp"
#include "Scene.hpp"
#include "Config.hpp"
#include <SDL3/SDL.h>
#include <memory>

class Game {
public:
    ~Game();

    // Singleton access to Game instance
    static Game& GetInstance() {
        static Game instance;
        return instance;
    }

public:
    // Initialize the game
    Error Init();

    // Cleanup game resources
    Error DeInit();

    // Run the main game loop
    void Run();

    // Get window and renderer
    SDL_Window* GetWindow() const { return m_window; }
    SDL_Renderer* GetRenderer() const { return m_renderer; }

    // Get window dimensions
    int GetScreenWidth() const { return kWindowWidth; }
    int GetScreenHeight() const { return kWindowHeight; }

    // Change to a new scene
    // scene: new scene object (unique_ptr, auto-managed)
    Error ChangeScene(std::unique_ptr<Scene> scene);

    // Exit the game
    void Exit() { m_isRunning = false; }

    // Check if game is running
    bool IsRunning() const { return m_isRunning; }

private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

private:
    void HandleEvent();
    void Update(float deltaTime);
    void Render();

private:
    bool m_isRunning = false;
    std::unique_ptr<Scene> m_currentScene;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};

#endif
