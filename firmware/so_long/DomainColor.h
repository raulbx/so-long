#pragma once

#include <stdint.h>

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  constexpr Color() : red(0), green(0), blue(0) {}
  constexpr Color(uint8_t r, uint8_t g, uint8_t b)
      : red(r), green(g), blue(b) {}
};

namespace SoLongColors {

constexpr Color Black{0, 0, 0};
constexpr Color Blue{0, 0, 255};
constexpr Color Red{255, 0, 0};
constexpr Color Orange{255,140,0};
constexpr Color Green{0, 180, 0};
constexpr Color Purple{180, 0, 180};
constexpr Color Yellow{255, 255, 0};
constexpr Color DeepSkyBlue{0, 191, 255};
constexpr Color Cyan{0, 255, 255};

}  // namespace SoLongColors
