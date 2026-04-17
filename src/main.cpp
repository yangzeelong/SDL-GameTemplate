#include "Game/Game.hpp"
#include "Game/SceneDemo.hpp"

// ============================================
// Entry Point
// ============================================

int main(int, char**) {
    auto& game = Game::GetInstance();

    // Initialize
    Error err = game.Init();
    if (err != Error::SUCCESS) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing game");
        return -1;
    }

    // Set initial scene
    game.ChangeScene(std::make_unique<SceneDemo>());

    // Run main loop
    game.Run();

    // Cleanup
    game.DeInit();
    return 0;
}
