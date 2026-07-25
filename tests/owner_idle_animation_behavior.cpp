#include <assert.h>

#include "AnimationEngine.h"
#include "DomainColor.h"
#include "Config.h"
#include "Emotion.h"
#include "EmotionalState.h"
#include "Identity.h"
#include "OwnerIdentity.h"

bool sameColor(CRGB left, CRGB right) {
  return left.r == right.r && left.g == right.g && left.b == right.b;
}

bool channelPatternMatches(CRGB rendered, CRGB source) {
  return (source.r == 0 ? rendered.r == 0 : rendered.r > 0) &&
         (source.g == 0 ? rendered.g == 0 : rendered.g > 0) &&
         (source.b == 0 ? rendered.b == 0 : rendered.b > 0);
}

bool sameColor(Color left, Color right) {
  return left.red == right.red && left.green == right.green &&
         left.blue == right.blue;
}

bool channelPatternMatches(CRGB rendered, Color source) {
  return (source.red == 0 ? rendered.r == 0 : rendered.r > 0) &&
         (source.green == 0 ? rendered.g == 0 : rendered.g > 0) &&
         (source.blue == 0 ? rendered.b == 0 : rendered.b > 0);
}

uint8_t countRedOverlayPixels(CRGB* leds, uint8_t ledCount) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < ledCount; i++) {
    if (leds[i].r > 0) {
      count++;
    }
  }
  return count;
}

uint8_t brightestRedPixel(CRGB* leds, uint8_t ledCount) {
  uint8_t brightest = 0;
  for (uint8_t i = 0; i < ledCount; i++) {
    if (leds[i].r > brightest) {
      brightest = leds[i].r;
    }
  }
  return brightest;
}

EmotionalState state(Emotion emotion, Color ownerColor, bool friendPresent,
                     Color friendColor, uint8_t intensity, uint8_t effectSize,
                     uint16_t motionIntervalMs) {
  return {
      emotion,
      ownerColor,
      friendPresent,
      friendColor,
      intensity,
      effectSize,
      motionIntervalMs,
  };
}

int main() {
  const FriendInfo* owner = localOwnerInfo();
  assert(owner != nullptr);
  assert(owner->id == MY_FRIEND);

#if SO_LONG_BOARD_ID == 1
  assert(MY_FRIEND == FriendId::RAHUL);
  assert(sameColor(owner->color, SoLongColors::Blue));
#elif SO_LONG_BOARD_ID == 2
  assert(MY_FRIEND == FriendId::JENNIFER);
  assert(sameColor(owner->color, SoLongColors::Red));
#else
#error Unsupported SO_LONG_BOARD_ID in owner idle animation test.
#endif

  CRGB leds[4];
  AnimationEngine animation(leds, 4);
  animation.begin();

  const Color staleFriendColor =
      MY_FRIEND == FriendId::RAHUL ? SoLongColors::Red : SoLongColors::Blue;
  animation.setEmotionalState(state(Emotion::IDLE, owner->color, false,
                                    staleFriendColor,
                                    SoLongConfig::COMET_CLOSE_INTENSITY,
                                    SoLongConfig::COMET_CLOSE_LENGTH,
                                    SoLongConfig::COMET_FAST_MS));

  fake_millis = SoLongConfig::ANIMATION_FRAME_MS;
  animation.update();

  for (uint8_t i = 0; i < 4; i++) {
    assert(channelPatternMatches(leds[i], owner->color));
    assert(!sameColor(leds[i], CRGB::Black));
    assert(!channelPatternMatches(leds[i], staleFriendColor));
  }

  CRGB overlayLeds[12];
  AnimationEngine overlayAnimation(overlayLeds, 12);
  overlayAnimation.begin();
  overlayAnimation.setEmotionalState(state(
      Emotion::PRESENT, SoLongColors::Blue, true, SoLongColors::Red,
      SoLongConfig::COMET_FAR_INTENSITY, SoLongConfig::COMET_FAR_LENGTH,
      SoLongConfig::COMET_SLOW_MS));

  fake_millis += SoLongConfig::ANIMATION_FRAME_MS;
  overlayAnimation.update();

  const uint8_t farOverlayPixels = countRedOverlayPixels(overlayLeds, 12);
  const uint8_t farBrightestPixel = brightestRedPixel(overlayLeds, 12);
  assert(farOverlayPixels > 0);

  for (uint8_t i = 0; i < 12; i++) {
    assert(overlayLeds[i].b > 0);
  }

  CRGB closeLeds[12];
  AnimationEngine closeAnimation(closeLeds, 12);
  closeAnimation.begin();
  closeAnimation.setEmotionalState(state(
      Emotion::PRESENT, SoLongColors::Blue, true, SoLongColors::Red,
      SoLongConfig::COMET_CLOSE_INTENSITY, SoLongConfig::COMET_CLOSE_LENGTH,
      SoLongConfig::COMET_FAST_MS));

  fake_millis += SoLongConfig::ANIMATION_FRAME_MS;
  closeAnimation.update();

  const uint8_t closeOverlayPixels = countRedOverlayPixels(closeLeds, 12);
  const uint8_t closeBrightestPixel = brightestRedPixel(closeLeds, 12);
  assert(closeOverlayPixels > farOverlayPixels);
  assert(closeBrightestPixel > farBrightestPixel);

  for (uint8_t i = 0; i < 12; i++) {
    assert(closeLeds[i].b > 0);
  }

  CRGB presentNearLeds[12];
  AnimationEngine presentNearAnimation(presentNearLeds, 12);
  presentNearAnimation.begin();
  presentNearAnimation.setEmotionalState(state(
      Emotion::PRESENT, SoLongColors::Blue, true, SoLongColors::Red,
      SoLongConfig::COMET_NEAR_INTENSITY, SoLongConfig::COMET_NEAR_LENGTH,
      SoLongConfig::COMET_MEDIUM_MS));

  fake_millis += SoLongConfig::ANIMATION_FRAME_MS;
  presentNearAnimation.update();

  CRGB approachingLeds[12];
  AnimationEngine approachingAnimation(approachingLeds, 12);
  approachingAnimation.begin();
  approachingAnimation.setEmotionalState(state(
      Emotion::APPROACHING, SoLongColors::Blue, true, SoLongColors::Red,
      SoLongConfig::COMET_NEAR_INTENSITY +
          SoLongConfig::APPROACHING_INTENSITY_BOOST,
      SoLongConfig::COMET_NEAR_LENGTH +
          SoLongConfig::APPROACHING_EFFECT_SIZE_BOOST,
      SoLongConfig::COMET_MEDIUM_MS -
          SoLongConfig::APPROACHING_MOTION_INTERVAL_REDUCTION_MS));

  fake_millis += SoLongConfig::ANIMATION_FRAME_MS;
  approachingAnimation.update();

  assert(countRedOverlayPixels(approachingLeds, 12) >
         countRedOverlayPixels(presentNearLeds, 12));
  assert(brightestRedPixel(approachingLeds, 12) >
         brightestRedPixel(presentNearLeds, 12));

  for (uint8_t i = 0; i < 12; i++) {
    assert(approachingLeds[i].b > 0);
  }

  return 0;
}
