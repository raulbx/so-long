#pragma once

#include <stdint.h>

struct CRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  constexpr CRGB() : r(0), g(0), b(0) {}
  constexpr CRGB(uint8_t red, uint8_t green, uint8_t blue)
      : r(red), g(green), b(blue) {}

  void nscale8_video(uint8_t scale) {
    r = scaleChannel(r, scale);
    g = scaleChannel(g, scale);
    b = scaleChannel(b, scale);
  }

  CRGB& fadeToBlackBy(uint8_t amount) {
    nscale8_video(255 - amount);
    return *this;
  }

  CRGB& operator+=(const CRGB& other) {
    r = saturatingAdd(r, other.r);
    g = saturatingAdd(g, other.g);
    b = saturatingAdd(b, other.b);
    return *this;
  }

  static const CRGB Black;
  static const CRGB Blue;
  static const CRGB Red;
  static const CRGB Orange;
  static const CRGB Green;
  static const CRGB Purple;
  static const CRGB Yellow;
  static const CRGB White;
  static const CRGB DeepSkyBlue;

 private:
  static uint8_t scaleChannel(uint8_t value, uint8_t scale) {
    if (value == 0) {
      return 0;
    }
    const uint16_t scaled = (static_cast<uint16_t>(value) * scale) / 255;
    return scaled == 0 ? 1 : static_cast<uint8_t>(scaled);
  }

  static uint8_t saturatingAdd(uint8_t left, uint8_t right) {
    const uint16_t sum = static_cast<uint16_t>(left) + right;
    return sum > 255 ? 255 : static_cast<uint8_t>(sum);
  }
};

inline const CRGB CRGB::Black = CRGB(0, 0, 0);
inline const CRGB CRGB::Blue = CRGB(0, 0, 255);
inline const CRGB CRGB::Red = CRGB(255, 0, 0);
inline const CRGB CRGB::Orange = CRGB(255, 165, 0);
inline const CRGB CRGB::Green = CRGB(0, 128, 0);
inline const CRGB CRGB::Purple = CRGB(128, 0, 128);
inline const CRGB CRGB::Yellow = CRGB(255, 255, 0);
inline const CRGB CRGB::White = CRGB(255, 255, 255);
inline const CRGB CRGB::DeepSkyBlue = CRGB(0, 191, 255);

class FastLEDClass {
 public:
  void setBrightness(uint8_t brightness) {
    brightness_ = brightness;
  }

  void clear(bool writeData = false) {
    if (writeData) {
      show();
    }
  }

  void show() {
    showCount++;
  }

  uint8_t brightness_ = 0;
  uint16_t showCount = 0;
};

inline FastLEDClass FastLED;

inline void fill_solid(CRGB* leds, uint16_t count, CRGB color) {
  for (uint16_t i = 0; i < count; i++) {
    leds[i] = color;
  }
}

inline void fadeToBlackBy(CRGB* leds, uint16_t count, uint8_t amount) {
  for (uint16_t i = 0; i < count; i++) {
    leds[i].fadeToBlackBy(amount);
  }
}

inline uint8_t beatsin8(uint8_t, uint8_t lowest = 0, uint8_t highest = 255) {
  return static_cast<uint8_t>((static_cast<uint16_t>(lowest) + highest) / 2);
}

inline uint16_t random16(uint16_t limit) {
  return limit == 0 ? 0 : 0;
}
