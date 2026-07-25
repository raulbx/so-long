#include <assert.h>

#include "AnimationEngine.h"
#include "Config.h"
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

int main() {
  const FriendInfo* owner = localOwnerInfo();
  assert(owner != nullptr);
  assert(owner->id == MY_FRIEND);

#if SO_LONG_BOARD_ID == 1
  assert(MY_FRIEND == FriendId::RAHUL);
  assert(sameColor(owner->color, CRGB::Blue));
#elif SO_LONG_BOARD_ID == 2
  assert(MY_FRIEND == FriendId::JENNIFER);
  assert(sameColor(owner->color, CRGB::Red));
#else
#error Unsupported SO_LONG_BOARD_ID in owner idle animation test.
#endif

  CRGB leds[4];
  AnimationEngine animation(leds, 4);
  animation.begin();

  const CRGB staleFriendColor =
      MY_FRIEND == FriendId::RAHUL ? CRGB::Red : CRGB::Blue;
  animation.setFriendColor(staleFriendColor);
  animation.setFriendDistanceMeters(0.75f);
  animation.setOwnerColor(owner->color);
  animation.setHeartState(HeartState::AMBIENT);

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
  overlayAnimation.setOwnerColor(CRGB::Blue);
  overlayAnimation.setFriendColor(CRGB::Red);
  overlayAnimation.setFriendDistanceMeters(4.0f);
  overlayAnimation.setHeartState(HeartState::FRIEND_DETECTED);

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
  closeAnimation.setOwnerColor(CRGB::Blue);
  closeAnimation.setFriendColor(CRGB::Red);
  closeAnimation.setFriendDistanceMeters(0.75f);
  closeAnimation.setHeartState(HeartState::FRIEND_FOUND);

  fake_millis += SoLongConfig::ANIMATION_FRAME_MS;
  closeAnimation.update();

  const uint8_t closeOverlayPixels = countRedOverlayPixels(closeLeds, 12);
  const uint8_t closeBrightestPixel = brightestRedPixel(closeLeds, 12);
  assert(closeOverlayPixels > farOverlayPixels);
  assert(closeBrightestPixel > farBrightestPixel);

  for (uint8_t i = 0; i < 12; i++) {
    assert(closeLeds[i].b > 0);
  }

  return 0;
}
