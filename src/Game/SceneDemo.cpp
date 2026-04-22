#include "SceneDemo.hpp"
#include "Game.hpp"
#include <SDL3/SDL.h>
#include <cmath>

SceneDemo::SceneDemo() : m_game(Game::GetInstance()), m_initialized(false), m_rotation(0.0f), m_time(0.0f) {}

SceneDemo::~SceneDemo() { DeInit(); }

Error SceneDemo::Init() {
    m_initialized = true;
    return Error::SUCCESS;
}

Error SceneDemo::DeInit() {
    m_initialized = false;
    return Error::SUCCESS;
}

void SceneDemo::Update(float deltaTime) {
    m_time += deltaTime;
    m_rotation += deltaTime * 1.0f;
}

void SceneDemo::Render() {
    SDL_Renderer* renderer = m_game.GetRenderer();

    // Dark background
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    // Center of the cube
    float centerX = static_cast<float>(m_game.GetScreenWidth()) / 2.0f;
    float centerY = static_cast<float>(m_game.GetScreenHeight()) / 2.0f;
    float cubeSize = 100.0f;

    // Cube vertices before rotation
    float vertices[8][3] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

    // Transform and project vertices
    float projected[8][2];

    for (int i = 0; i < 8; i++) {
        float x = vertices[i][0];
        float y = vertices[i][1];
        float z = vertices[i][2];

        // Rotate around Y axis
        float cosR = std::cos(m_rotation);
        float sinR = std::sin(m_rotation);
        float rx = x * cosR - z * sinR;
        float rz = x * sinR + z * cosR;

        // Rotate around X axis
        float ry = y * cosR * 0.7f - rz * sinR * 0.7f;
        float rz2 = y * sinR * 0.7f + rz * cosR;

        // Simple perspective projection
        float scale = 2.5f / (2.5f + rz2);
        projected[i][0] = centerX + rx * cubeSize * scale;
        projected[i][1] = centerY + ry * cubeSize * scale;
    }

    // Draw filled cube faces with triangles
    int faces[6][4] = {
        {0, 1, 2, 3}, // front
        {4, 5, 6, 7}, // back
        {0, 1, 5, 4}, // bottom
        {2, 3, 7, 6}, // top
        {0, 3, 7, 4}, // left
        {1, 2, 6, 5}  // right
    };

    for (int f = 0; f < 6; f++) {
        // Calculate color based on z = x^2 + y^2
        float avgZ = 0.0f;
        for (int j = 0; j < 4; j++) {
            float x = vertices[faces[f][j]][0];
            float y = vertices[faces[f][j]][1];
            avgZ += x * x + y * y;
        }
        avgZ /= 4.0f;

        // Animated color (float for SDL_FColor)
        float t = std::fmod(avgZ + m_time * 0.5f, 1.0f);
        float cr = 0.3f + 0.7f * std::sin(t * 3.14159f);
        float cg = 0.3f + 0.7f * std::sin(t * 3.14159f + 2.0f);
        float cb = 0.3f + 0.7f * std::sin(t * 3.14159f + 4.0f);

        // Triangle 1: v0, v1, v2
        SDL_Vertex tri1[3] = {
            {{projected[faces[f][0]][0], projected[faces[f][0]][1]}, {cr, cg, cb, 1.0f}, {0, 0}},
            {{projected[faces[f][1]][0], projected[faces[f][1]][1]}, {cr, cg, cb, 1.0f}, {0, 0}},
            {{projected[faces[f][2]][0], projected[faces[f][2]][1]}, {cr, cg, cb, 1.0f}, {0, 0}}};
        SDL_RenderGeometry(renderer, nullptr, tri1, 3, nullptr, 0);

        // Triangle 2: v0, v2, v3
        SDL_Vertex tri2[3] = {
            {{projected[faces[f][0]][0], projected[faces[f][0]][1]}, {cr, cg, cb, 1.0f}, {0, 0}},
            {{projected[faces[f][2]][0], projected[faces[f][2]][1]}, {cr, cg, cb, 1.0f}, {0, 0}},
            {{projected[faces[f][3]][0], projected[faces[f][3]][1]}, {cr, cg, cb, 1.0f}, {0, 0}}};
        SDL_RenderGeometry(renderer, nullptr, tri2, 3, nullptr, 0);
    }

    // Draw wireframe on top
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int edges[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
    for (int e = 0; e < 12; e++) {
        SDL_RenderLine(renderer, projected[edges[e][0]][0], projected[edges[e][0]][1], projected[edges[e][1]][0],
                      projected[edges[e][1]][1]);
    }
}

void SceneDemo::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            m_game.Quit();
        }
    }
}
