#include "AnimationEngine.h"

#include "Config.h"

AnimationEngine::AnimationEngine(CRGB* leds, uint16_t ledCount)
    : leds_(leds),
      ledCount_(ledCount),
      state_({
          Emotion::IDLE,
          SoLongColors::DeepSkyBlue,
          false,
          SoLongColors::Black,
          0,
          0,
          SoLongConfig::COMET_SLOW_MS,
      }),
      lastFrameMs_(0),
      lastCometStepMs_(0),
      frame_(0),
      cometPosition_(0) {}

void AnimationEngine::begin() {
  FastLED.setBrightness(SoLongConfig::LED_BRIGHTNESS);
  FastLED.clear(true);
}

void AnimationEngine::setEmotionalState(EmotionalState state) {
  state_ = state;
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

  switch (state_.emotion) {
    case Emotion::IDLE:
      renderAmbientBreath();
      break;
    case Emotion::PRESENT:
      renderComet();
      break;
    case Emotion::APPROACHING:
      renderApproachingComet();
      break;
    case Emotion::CURIOUS:
      renderCuriousAcknowledgement();
      break;
    case Emotion::REUNITED:
      renderReunitedCelebration();
      break;
    case Emotion::DEPARTING:
      renderDepartingFade();
      break;
  }

  FastLED.show();
}

void AnimationEngine::renderComet() {
  const uint32_t now = millis();
  renderOwnerBreathBackground();

  renderCometOverlay(state_.effectSize, state_.intensity);
  advanceCometIfDue(now, state_.motionIntervalMs);
}

void AnimationEngine::renderAmbientBreath() {
  renderOwnerBreathBackground();
}

void AnimationEngine::renderFoundSparkle() {
  renderReunitedCelebration();
}

void AnimationEngine::renderOwnerBreathBackground() {
  const uint8_t breath = beatsin8(6, 18, 96);
  CRGB base = toCrgb(state_.ownerColor);
  base.nscale8_video(breath);

  fill_solid(leds_, ledCount_, base);
}

void AnimationEngine::renderApproachingComet() {
  renderComet();
}

void AnimationEngine::renderCuriousAcknowledgement() {
  const uint32_t now = millis();
  renderOwnerBreathBackground();
  renderCometOverlay(state_.effectSize, state_.intensity);
  advanceCometIfDue(now, state_.motionIntervalMs);
}

void AnimationEngine::renderReunitedCelebration() {
  const uint32_t now = millis();
  renderOwnerBreathBackground();

  const uint16_t burstSize =
      state_.effectSize < ledCount_ ? state_.effectSize : ledCount_;
  for (uint16_t segment = 0; segment < burstSize; segment++) {
    const uint16_t forwardIndex = (cometPosition_ + segment) % ledCount_;
    const uint16_t reverseIndex =
        (cometPosition_ + ledCount_ - segment) % ledCount_;

    CRGB friendOverlay = toCrgb(state_.friendColor);
    friendOverlay.nscale8_video(state_.intensity);
    leds_[forwardIndex] += friendOverlay;

    CRGB ownerSpark = toCrgb(state_.ownerColor);
    ownerSpark.nscale8_video(state_.intensity / 2);
    leds_[reverseIndex] += ownerSpark;
  }

  advanceCometIfDue(now, state_.motionIntervalMs);
}

void AnimationEngine::renderDepartingFade() {
  const uint32_t now = millis();
  renderOwnerBreathBackground();
  renderCometOverlay(state_.effectSize, state_.intensity);
  advanceCometIfDue(now, state_.motionIntervalMs);
}

void AnimationEngine::renderCometOverlay(uint8_t length, uint8_t brightness) {
  if (length == 0 || brightness == 0 || ledCount_ == 0) {
    return;
  }

  const uint16_t cometLength = length < ledCount_ ? length : ledCount_;

  for (uint16_t segment = 0; segment < cometLength; segment++) {
    const uint16_t index = (cometPosition_ + ledCount_ - segment) % ledCount_;
    const uint8_t segmentBrightness = static_cast<uint8_t>(
        (static_cast<uint16_t>(brightness) * (cometLength - segment)) /
        cometLength);

    CRGB overlay = toCrgb(state_.friendColor);
    overlay.nscale8_video(segmentBrightness);
    leds_[index] += overlay;
  }
}

void AnimationEngine::advanceCometIfDue(uint32_t now,
                                        uint16_t motionIntervalMs) {
  const uint16_t interval = motionIntervalMs == 0 ? 1 : motionIntervalMs;
  if (now - lastCometStepMs_ < interval) {
    return;
  }

  lastCometStepMs_ = now;
  cometPosition_++;
  if (cometPosition_ >= ledCount_) {
    cometPosition_ = 0;
  }
}

CRGB AnimationEngine::toCrgb(Color color) const {
  return CRGB(color.red, color.green, color.blue);
}
