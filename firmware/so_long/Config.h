#pragma once

#include <Arduino.h>
#include <FastLED.h>

namespace SoLongConfig {

constexpr uint8_t LED_DATA_PIN = 25;
constexpr uint16_t LED_COUNT = 50;
constexpr uint8_t LED_BRIGHTNESS = 96;
constexpr EOrder LED_COLOR_ORDER = GRB;

constexpr uint32_t ANIMATION_FRAME_MS = 20;

constexpr float FRIEND_DETECTED_DISTANCE_M = 12.0f;
constexpr float FRIEND_NEAR_DISTANCE_M = 5.0f;
constexpr float FRIEND_FOUND_DISTANCE_M = 1.5f;
constexpr uint32_t FRIEND_TIMEOUT_MS = 3000;

constexpr uint16_t COMET_FAST_MS = 20;
constexpr uint16_t COMET_MEDIUM_MS = 45;
constexpr uint16_t COMET_SLOW_MS = 90;

}  // namespace SoLongConfig
