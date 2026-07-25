#include "AnimationEngine.h"

#include "Config.h"

AnimationEngine::AnimationEngine(CRGB* leds, uint16_t ledCount)
    : leds_(leds),
      ledCount_(ledCount),
      ownerColor_(CRGB::DeepSkyBlue),
      friendColor_(CRGB::DeepSkyBlue),
      friendDistanceMeters_(4.0f),
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

void AnimationEngine::setOwnerColor(CRGB color) {
  ownerColor_ = color;
}

void AnimationEngine::setFriendColor(CRGB color) {
  friendColor_ = color;
}

void AnimationEngine::setFriendDistanceMeters(float distanceMeters) {
  friendDistanceMeters_ = distanceMeters;
  setCometSpeedMs(cometSpeedForDistance(distanceMeters));
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
    case HeartState::FRIEND_FOUND:
      renderComet();
      break;
  }

  FastLED.show();
}

void AnimationEngine::renderComet() {
  const uint32_t now = millis();
  renderOwnerBreathBackground();

  const uint8_t requestedLength = cometLengthForDistance(friendDistanceMeters_);
  const uint16_t cometLength =
      requestedLength < ledCount_ ? requestedLength : ledCount_;
  const uint8_t cometBrightness =
      cometBrightnessForDistance(friendDistanceMeters_);

  for (uint16_t segment = 0; segment < cometLength; segment++) {
    const uint16_t index = (cometPosition_ + ledCount_ - segment) % ledCount_;
    const uint8_t segmentBrightness = static_cast<uint8_t>(
        (static_cast<uint16_t>(cometBrightness) * (cometLength - segment)) /
        cometLength);

    CRGB overlay = friendColor_;
    overlay.nscale8_video(segmentBrightness);
    leds_[index] += overlay;
  }

  if (now - lastCometStepMs_ < cometSpeedMs_) {
    return;
  }

  lastCometStepMs_ = now;

  cometPosition_++;
  if (cometPosition_ >= ledCount_) {
    cometPosition_ = 0;
  }
}

void AnimationEngine::renderAmbientBreath() {
  renderOwnerBreathBackground();
}

void AnimationEngine::renderOwnerBreathBackground() {
  const uint8_t breath = beatsin8(6, 18, 96);
  CRGB base = ownerColor_;
  base.nscale8_video(breath);

  fill_solid(leds_, ledCount_, base);
}

void AnimationEngine::renderFoundSparkle() {
  fadeToBlackBy(leds_, ledCount_, 64);

  const uint8_t sparkleCount = ledCount_ / 10 > 3 ? ledCount_ / 10 : 3;
  for (uint8_t i = 0; i < sparkleCount; i++) {
    const uint16_t index = random16(ledCount_);
    leds_[index] += friendColor_;
  }
}

uint8_t AnimationEngine::cometLengthForDistance(float distanceMeters) const {
  if (distanceMeters < 1.0f) {
    return 10;
  }

  if (distanceMeters < 3.0f) {
    return 6;
  }

  return 3;
}

uint8_t AnimationEngine::cometBrightnessForDistance(
    float distanceMeters) const {
  if (distanceMeters < 1.0f) {
    return 240;
  }

  if (distanceMeters < 3.0f) {
    return 180;
  }

  return 110;
}

uint16_t AnimationEngine::cometSpeedForDistance(float distanceMeters) const {
  if (distanceMeters < 1.0f) {
    return SoLongConfig::COMET_FAST_MS;
  }

  if (distanceMeters < 3.0f) {
    return SoLongConfig::COMET_MEDIUM_MS;
  }

  return SoLongConfig::COMET_SLOW_MS;
}
