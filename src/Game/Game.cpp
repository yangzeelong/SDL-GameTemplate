#include "Game.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>

Game::~Game() { DeInit(); }

Error Game::Init() {
    // Initialize settings from config file
    Settings::Init();

    // Initialize SDL video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed");
        return Error::ERROR_INIT;
    }

    // Create window
    m_window = SDL_CreateWindow(Settings::WindowTitle, Settings::WindowWidth, Settings::WindowHeight, 0);
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

    // Initialize TTF for FPS display
    if (!TTF_WasInit() && !TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed");
        return Error::ERROR_INIT;
    }

    // Load FPS font
    if (Settings::ShowFPS) {
        m_fpsFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 16);
        if (!m_fpsFont) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_OpenFont failed: %s", SDL_GetError());
            return Error::ERROR_INIT;
        }
    }

    m_isRunning = true;
    return Error::SUCCESS;
}

Error Game::DeInit() {
    // Release current scene
    m_currentScene.reset();

    // Release FPS font
    if (m_fpsFont) {
        TTF_CloseFont(m_fpsFont);
        m_fpsFont = nullptr;
    }

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
    uint64_t lastTime = SDL_GetTicksNS();
    m_fpsTimer = SDL_GetTicksNS();
    m_frameCount = 0;
    m_fps = 0;

    while (m_isRunning) {
        uint64_t currentTime = SDL_GetTicksNS();
        float deltaTime = static_cast<float>((currentTime - lastTime) / 1'000'000'000.0);
        lastTime = currentTime;

        HandleEvent();
        Update(deltaTime);
        Render();

        // Frame rate control
        uint64_t frameTime = SDL_GetTicksNS() - currentTime;
        if (frameTime < Settings::FrameDelay * 1'000'000) {
            SDL_Delay(static_cast<Uint32>((Settings::FrameDelay * 1'000'000 - frameTime) / 1'000'000));
        }

        // FPS calculation
        m_frameCount++;
        uint64_t elapsed = SDL_GetTicksNS() - m_fpsTimer;
        if (elapsed > 0) {
            m_fps = static_cast<int>(m_frameCount * 1'000'000'000.0 / elapsed + 0.5);
        }
        if (elapsed >= 1'000'000'000) {
            m_frameCount = 0;
            m_fpsTimer = SDL_GetTicksNS();
        }
    }
}

void Game::HandleEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Quit event - Quit game
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

    // Render FPS if enabled
    if (Settings::ShowFPS && m_fpsFont) {
        int displayFPS = m_fps > 0 ? m_fps : m_frameCount;
        char fpsText[32];
        SDL_snprintf(fpsText, sizeof(fpsText), "FPS: %d", displayFPS);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(m_fpsFont, fpsText, SDL_strlen(fpsText), white);
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, surf);
            if (tex) {
                SDL_FRect dst = {10.0f, 10.0f, static_cast<float>(surf->w), static_cast<float>(surf->h)};
                SDL_RenderTexture(m_renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_DestroySurface(surf);
        }
    }

    // Present to screen (must be called only once, here in Game)
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
