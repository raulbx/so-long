#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "HeartState.h"

class AnimationEngine {
 public:
  AnimationEngine(CRGB* leds, uint16_t ledCount);

  void begin();
  void setFriendColor(CRGB color);
  void setHeartState(HeartState state);
  void setCometSpeedMs(uint16_t speedMs);
  void update();

  void renderComet();
  void renderAmbientBreath();
  void renderFoundSparkle();

 private:
  CRGB* leds_;
  uint16_t ledCount_;
  CRGB friendColor_;
  HeartState heartState_;
  uint32_t lastFrameMs_;
  uint32_t lastCometStepMs_;
  uint16_t frame_;
  uint16_t cometPosition_;
  uint16_t cometSpeedMs_;
};
