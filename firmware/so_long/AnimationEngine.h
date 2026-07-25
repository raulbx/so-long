#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "DomainColor.h"
#include "EmotionalState.h"

class AnimationEngine {
 public:
  AnimationEngine(CRGB* leds, uint16_t ledCount);

  void begin();
  void setEmotionalState(EmotionalState state);
  void update();

  void renderComet();
  void renderAmbientBreath();
  void renderFoundSparkle();

 private:
  CRGB* leds_;
  uint16_t ledCount_;
  EmotionalState state_;
  uint32_t lastFrameMs_;
  uint32_t lastCometStepMs_;
  uint16_t frame_;
  uint16_t cometPosition_;

  void renderOwnerBreathBackground();
  void renderCuriousAcknowledgement();
  void renderReunitedCelebration();
  void renderDepartingFade();
  void renderCometOverlay(uint8_t length, uint8_t brightness);
  void advanceCometIfDue(uint32_t now, uint16_t motionIntervalMs);
  CRGB toCrgb(Color color) const;
};
