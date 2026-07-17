#include "AnimationEngine.h"

#include "Config.h"

AnimationEngine::AnimationEngine(CRGB* leds, uint16_t ledCount)
    : leds_(leds),
      ledCount_(ledCount),
      friendColor_(CRGB::DeepSkyBlue),
      heartState_(HeartState::AMBIENT),
      lastFrameMs_(0),
      lastCometStepMs_(0),
      frame_(0),
      cometPosition_(0),
      cometSpeedMs_(90) {}

void AnimationEngine::begin() {
  FastLED.setBrightness(SoLongConfig::LED_BRIGHTNESS);
  FastLED.clear(true);
}

void AnimationEngine::setFriendColor(CRGB color) {
  friendColor_ = color;
}

void AnimationEngine::setHeartState(HeartState state) {
  heartState_ = state;
}

void AnimationEngine::setCometSpeedMs(uint16_t speedMs) {
  cometSpeedMs_ = speedMs == 0 ? 1 : speedMs;
}

void AnimationEngine::update() {
  if (leds_ == nullptr || ledCount_ == 0) {
    return;
  }

  const uint32_t now = millis();
  if (now - lastFrameMs_ < SoLongConfig::ANIMATION_FRAME_MS) {
    return;
  }

  lastFrameMs_ = now;
  frame_++;

  switch (heartState_) {
    case HeartState::AMBIENT:
      renderAmbientBreath();
      break;
    case HeartState::FRIEND_DETECTED:
    case HeartState::FRIEND_NEAR:
      renderComet();
      break;
    case HeartState::FRIEND_FOUND:
      renderFoundSparkle();
      break;
  }

  FastLED.show();
}

void AnimationEngine::renderComet() {
  const uint32_t now = millis();
  if (now - lastCometStepMs_ < cometSpeedMs_) {
    return;
  }
  lastCometStepMs_ = now;

  fadeToBlackBy(leds_, ledCount_, 20);

  leds_[cometPosition_] = friendColor_;

  if (cometPosition_ > 0) {
    CRGB tail = friendColor_;
    tail.fadeToBlackBy(80);
    leds_[cometPosition_ - 1] += tail;
  }

  if (cometPosition_ > 1) {
    CRGB tail = friendColor_;
    tail.fadeToBlackBy(160);
    leds_[cometPosition_ - 2] += tail;
  }

  cometPosition_++;
  if (cometPosition_ >= ledCount_) {
    cometPosition_ = 0;
  }
}

void AnimationEngine::renderAmbientBreath() {
  const uint8_t breath = beatsin8(10, 12, 90);
  const CRGB base = CRGB(8, 18, 28);

  fill_solid(leds_, ledCount_, base);
  for (uint16_t i = 0; i < ledCount_; i++) {
    leds_[i].nscale8_video(breath);
  }
}

void AnimationEngine::renderFoundSparkle() {
  fadeToBlackBy(leds_, ledCount_, 64);

  const uint8_t sparkleCount = ledCount_ / 10 > 3 ? ledCount_ / 10 : 3;
  for (uint8_t i = 0; i < sparkleCount; i++) {
    const uint16_t index = random16(ledCount_);
    leds_[index] += friendColor_;
  }
}
