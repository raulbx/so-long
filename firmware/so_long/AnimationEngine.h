#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "HeartState.h"

class AnimationEngine {
 public:
  AnimationEngine(CRGB* leds, uint16_t ledCount);

  void begin();
  void setOwnerColor(CRGB color);
  void setFriendColor(CRGB color);
  void setFriendDistanceMeters(float distanceMeters);
  void setHeartState(HeartState state);
  void setCometSpeedMs(uint16_t speedMs);
  void update();

  void renderComet();
  void renderAmbientBreath();
  void renderFoundSparkle();

 private:
  CRGB* leds_;
  uint16_t ledCount_;
  CRGB ownerColor_;
  CRGB friendColor_;
  float friendDistanceMeters_;
  HeartState heartState_;
  uint32_t lastFrameMs_;
  uint32_t lastCometStepMs_;
  uint16_t frame_;
  uint16_t cometPosition_;
  uint16_t cometSpeedMs_;

  void renderOwnerBreathBackground();
  uint8_t cometLengthForDistance(float distanceMeters) const;
  uint8_t cometBrightnessForDistance(float distanceMeters) const;
  uint16_t cometSpeedForDistance(float distanceMeters) const;
};
