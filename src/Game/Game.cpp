#include "Game.hpp"
#include "Config.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_log.h>

Game::~Game() {
    DeInit();
}

Error Game::Init() {
    // Initialize SDL video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed");
        return Error::ERROR_INIT;
    }

    // Create window
    m_window = SDL_CreateWindow(
        kWindowTitle,
        kWindowWidth,
        kWindowHeight,
        0);
    if (!m_window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed");
        return Error::ERROR_INIT;
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, NULL);
    if (!m_renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed");
        return Error::ERROR_INIT;
    }

    m_isRunning = true;
    return Error::SUCCESS;
}

Error Game::DeInit() {
    // Release current scene
    m_currentScene.reset();

    // Destroy renderer
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    // Destroy window
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    // Quit SDL
    SDL_Quit();
    return Error::SUCCESS;
}

void Game::Run() {
    uint32_t lastTime = SDL_GetTicks();

    while (m_isRunning) {
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        HandleEvent();
        Update(deltaTime);
        Render();

        // Frame rate control
        uint32_t frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < kFrameDelay) {
            SDL_Delay(static_cast<Uint32>(kFrameDelay - frameTime));
        }
    }
}

void Game::HandleEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Quit event - exit game
        if (event.type == SDL_EVENT_QUIT) {
            m_isRunning = false;
        }

        // Pass event to current scene
        m_currentScene->HandleEvent(event);
    }
}

void Game::Update(float deltaTime) {
    // Update current scene
    m_currentScene->Update(deltaTime);
}

void Game::Render() {
    // Clear screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Render current scene
    m_currentScene->Render();

    // Present to screen
    SDL_RenderPresent(m_renderer);
}

Error Game::ChangeScene(std::unique_ptr<Scene> scene) {
    // Release old scene
    if (m_currentScene) {
        m_currentScene->DeInit();
    }
    // Switch to new scene
    m_currentScene = std::move(scene);
    // Initialize new scene
    return m_currentScene->Init();
}
