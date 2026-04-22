#include "SceneDemo.hpp"
#include "Game.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <cstdio>

SceneDemo::SceneDemo()
    : m_game(Game::GetInstance()),
      m_font(nullptr),
      m_initialized(false),
      m_squareX(100.0f),
      m_squareY(100.0f),
      m_squareSpeed(200.0f),
      m_directionX(1),
      m_directionY(1) {}

SceneDemo::~SceneDemo() { DeInit(); }

Error SceneDemo::Init() {
    // Initialize TTF font
    if (!TTF_WasInit() && !TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed");
        return Error::ERROR_INIT;
    }

    // Load font
    m_font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!m_font) {
        m_font = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 24);
    }

    m_initialized = true;
    return Error::SUCCESS;
}

Error SceneDemo::DeInit() {
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
    m_initialized = false;
    return Error::SUCCESS;
}

void SceneDemo::Update(float deltaTime) {
    // Demo: move square and bounce off walls
    m_squareX += m_squareSpeed * m_directionX * deltaTime;
    m_squareY += m_squareSpeed * m_directionY * deltaTime;

    // Boundary collision - bounce
    int screenW = m_game.GetScreenWidth();
    int screenH = m_game.GetScreenHeight();
    int squareSize = 50;

    if (m_squareX <= 0 || m_squareX >= screenW - squareSize) {
        m_directionX *= -1;
        m_squareX = std::max(0.0f, std::min(m_squareX, static_cast<float>(screenW - squareSize)));
    }
    if (m_squareY <= 0 || m_squareY >= screenH - squareSize) {
        m_directionY *= -1;
        m_squareY = std::max(0.0f, std::min(m_squareY, static_cast<float>(screenH - squareSize)));
    }
}

void SceneDemo::Render() {
    SDL_Renderer* renderer = m_game.GetRenderer();

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    // Draw moving square
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_FRect rect = {m_squareX, m_squareY, 50, 50};
    SDL_RenderFillRect(renderer, &rect);

    // Draw instructions
    if (m_font) {
        const char* text = "SDL Game Template - Press ESC to Quit";
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(m_font, text, SDL_strlen(text), white);
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                float tw, th;
                SDL_GetTextureSize(tex, &tw, &th);
                float x = (m_game.GetScreenWidth() - tw) / 2.0f;
                SDL_FRect dst = {x, 20.0f, tw, th};
                SDL_RenderTexture(renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_DestroySurface(surf);
        }
    }
}

void SceneDemo::HandleEvent(SDL_Event& event) {
    // ESC to Quit
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            m_game.Quit();
        }
    }
}
