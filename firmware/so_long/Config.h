#pragma once

#include <Arduino.h>

#define SO_LONG_LED_COLOR_ORDER GRB

namespace SoLongConfig {

constexpr uint8_t LED_DATA_PIN = 25;
constexpr uint16_t LED_COUNT = 50;
constexpr uint8_t LED_BRIGHTNESS = 96;

constexpr uint32_t ANIMATION_FRAME_MS = 20;

constexpr float FRIEND_DETECTED_DISTANCE_M = 12.0f;
constexpr float FRIEND_NEAR_DISTANCE_M = 5.0f;
constexpr float FRIEND_FOUND_DISTANCE_M = 1.5f;
constexpr uint32_t FRIEND_TIMEOUT_MS = 3000;
constexpr uint32_t FRIEND_VISIBILITY_TIMEOUT_MS = 2000;

constexpr uint16_t COMET_FAST_MS = 20;
constexpr uint16_t COMET_MEDIUM_MS = 45;
constexpr uint16_t COMET_SLOW_MS = 90;

constexpr uint32_t CURIOUS_DURATION_MS = 900;
constexpr uint32_t REUNITED_DURATION_MS = 1200;
constexpr uint32_t DEPARTING_DURATION_MS = 1000;

constexpr float REUNITED_ENTER_DISTANCE_M = 2.0f;
constexpr float REUNITED_EXIT_DISTANCE_M = 2.5f;

constexpr uint32_t APPROACHING_WINDOW_MS = 1500;
constexpr float APPROACHING_ENTER_DELTA_M = 0.5f;
constexpr float APPROACHING_EXIT_DELTA_M = 0.2f;

constexpr uint8_t COMET_FAR_LENGTH = 3;
constexpr uint8_t COMET_NEAR_LENGTH = 6;
constexpr uint8_t COMET_CLOSE_LENGTH = 10;

constexpr uint8_t COMET_FAR_INTENSITY = 110;
constexpr uint8_t COMET_NEAR_INTENSITY = 180;
constexpr uint8_t COMET_CLOSE_INTENSITY = 240;

constexpr uint8_t CURIOUS_EFFECT_SIZE = 4;
constexpr uint8_t CURIOUS_INTENSITY = 96;
constexpr uint8_t REUNITED_EFFECT_SIZE = 12;
constexpr uint8_t REUNITED_INTENSITY = 255;
constexpr uint8_t DEPARTING_EFFECT_SIZE = 6;
constexpr uint8_t APPROACHING_EFFECT_SIZE_BOOST = 2;
constexpr uint8_t APPROACHING_INTENSITY_BOOST = 32;
constexpr uint16_t APPROACHING_MOTION_INTERVAL_REDUCTION_MS = 15;

}  // namespace SoLongConfig
