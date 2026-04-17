#ifndef CONFIG_HPP_
#define CONFIG_HPP_

// ============================================
// Window Configuration
// ============================================
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
constexpr const char* kWindowTitle = "SDL Game Template";

// ============================================
// Frame Rate
// ============================================
constexpr int kFPS = 60;
constexpr float kFrameDelay = 1000.0f / kFPS;

// ============================================
// Colors (RGBA) - normalized 0.0~1.0
// ============================================
struct Color {
    float r, g, b, a;
};

constexpr Color kColorBlack{0.0f, 0.0f, 0.0f, 1.0f};
constexpr Color kColorWhite{1.0f, 1.0f, 1.0f, 1.0f};
constexpr Color kColorRed{1.0f, 0.0f, 0.0f, 1.0f};
constexpr Color kColorGreen{0.0f, 1.0f, 0.0f, 1.0f};
constexpr Color kColorBlue{0.0f, 0.0f, 1.0f, 1.0f};
constexpr Color kColorGray{0.5f, 0.5f, 0.5f, 1.0f};

// ============================================
// Grid Configuration (for tile-based games)
// ============================================
constexpr int kGridCols = 20;
constexpr int kGridRows = 15;
constexpr int kCellSize = 32;

// ============================================
// Game Settings
// ============================================
// Add your game-specific settings here

#endif
