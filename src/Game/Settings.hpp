#ifndef SETTING_HPP_
#define SETTING_HPP_

// ============================================
// Game Settings - Global configuration values
// Loaded from config.ini at startup
// ============================================
namespace Settings {

extern int WindowWidth;
extern int WindowHeight;
extern const char* WindowTitle;
extern int FPS;
extern float FrameDelay;
extern bool ShowFPS;

// Initialize settings from config file
void Init();

} // namespace Settings

#endif // SETTING_HPP_
