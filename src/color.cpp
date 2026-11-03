#include <stdint.h>
#include "../include/color.h"


uint8_t Color::Clamp(int val) {
    if (val < 0) return 0;
    if (val > 255) return 255;
    return val;
}


Color::Color(uint8_t R, uint8_t G, uint8_t B) {
    this->R = R;
    this->G = G;
    this->B = B;
}


Color Color::operator+(const Color& oth) const {
    return Color(Clamp(R + oth.R), Clamp(G + oth.G), Clamp(B + oth.B));
}
Color Color::operator-(const Color& oth) const {
    return Color(Clamp(R - oth.R), Clamp(G - oth.G), Clamp(B - oth.B));
}
Color Color::operator*(const Color& oth) const {
    return Color(Clamp(R * oth.R), Clamp(G * oth.G), Clamp(B * oth.B));
}
Color Color::operator/(const Color& oth) const {
    return Color(
        oth.R ? Clamp(R / oth.R) : 255,
        oth.G ? Clamp(G / oth.G) : 255,
        oth.B ? Clamp(B / oth.B) : 255
    );
}

Color& Color::operator+=(const Color& oth) { *this = *this + oth; return *this; }
Color& Color::operator-=(const Color& oth) { *this = *this - oth; return *this; }
Color& Color::operator*=(const Color& oth) { *this = *this * oth; return *this; }
Color& Color::operator/=(const Color& oth) { *this = *this / oth; return *this; }


Color Color::operator+(uint8_t oth) const {
    return Color(Clamp(R + oth), Clamp(G + oth), Clamp(B + oth));
}
Color Color::operator-(uint8_t oth) const {
    return Color(Clamp(R - oth), Clamp(G - oth), Clamp(B - oth));
}
Color Color::operator*(uint8_t oth) const {
    return Color(Clamp(R * oth), Clamp(G * oth), Clamp(B * oth));
}
Color Color::operator/(uint8_t oth) const {
    if (oth == 0) return Color(255, 255, 255);
    return Color(Clamp(R / oth), Clamp(G / oth), Clamp(B / oth));
}

Color& Color::operator+=(uint8_t val) { *this = *this + val; return *this; }
Color& Color::operator-=(uint8_t val) { *this = *this - val; return *this; }
Color& Color::operator*=(uint8_t val) { *this = *this * val; return *this; }
Color& Color::operator/=(uint8_t val) { *this = *this / val; return *this; }

bool Color::operator==(const Color& oth) const { return R == oth.R && G == oth.G && B == oth.B; }
bool Color::operator!=(const Color& oth) const { return !(*this == oth); }
