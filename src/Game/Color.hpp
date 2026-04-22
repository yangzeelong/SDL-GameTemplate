#ifndef COLOR_HPP_
#define COLOR_HPP_

// ============================================
// Color - RGBA color values
// ============================================
struct Color {
    float r, g, b, a;

    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}

    // Normalized (0-1) to Uint8 (0-255)
    static Color FromRGB8(int red, int green, int blue, int alpha = 255) {
        return Color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    }
};

// ============================================
// Common Colors
// ============================================
namespace Colors {
const Color White(1.0f, 1.0f, 1.0f);
const Color Black(0.0f, 0.0f, 0.0f);
const Color Red(1.0f, 0.0f, 0.0f);
const Color Green(0.0f, 1.0f, 0.0f);
const Color Blue(0.0f, 0.0f, 1.0f);
const Color Yellow(1.0f, 1.0f, 0.0f);
const Color Cyan(0.0f, 1.0f, 1.0f);
const Color Magenta(1.0f, 0.0f, 1.0f);
const Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);
} // namespace Colors

#endif // COLOR_HPP_
