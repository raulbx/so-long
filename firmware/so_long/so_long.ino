#include <Arduino.h>
#include <FastLED.h>

#include "AnimationEngine.h"
#include "Config.h"
#include "FriendManager.h"
#include "Friends.h"
#include "Identity.h"
#include "UWBManager.h"

CRGB leds[SoLongConfig::LED_COUNT];
AnimationEngine animation(leds, SoLongConfig::LED_COUNT);
FriendManager friendManager;
UWBManager uwb;

FriendId activeFriendId = MY_FRIEND;

const FriendInfo* findFriend(FriendId id) {
  for (size_t i = 0; i < FRIEND_COUNT; i++) {
    if (FRIENDS[i].id == id) {
      return &FRIENDS[i];
    }
  }
  return nullptr;
}

uint16_t cometSpeedForDistance(float distanceM) {
  if (distanceM < 1.0f) {
    return SoLongConfig::COMET_FAST_MS;
  }
  if (distanceM < 3.0f) {
    return SoLongConfig::COMET_MEDIUM_MS;
  }
  return SoLongConfig::COMET_SLOW_MS;
}

void applyFriendToAnimation(const FriendObservation* observation,
                            HeartState heartState) {
  if (observation == nullptr) {
    animation.setHeartState(heartState);
    return;
  }

  activeFriendId = observation->id;
  const FriendInfo* friendInfo = findFriend(observation->id);
  if (friendInfo != nullptr) {
    animation.setFriendColor(friendInfo->color);
  }

  animation.setHeartState(heartState);
  animation.setCometSpeedMs(cometSpeedForDistance(observation->distanceM));
}

void setup() {
  FastLED.addLeds<WS2811, SoLongConfig::LED_DATA_PIN,
                  SO_LONG_LED_COLOR_ORDER>(
      leds, SoLongConfig::LED_COUNT);

  animation.begin();
  animation.setCometSpeedMs(SoLongConfig::COMET_SLOW_MS);
  friendManager.begin();

  uwb.begin();
}

void loop() {
  const uint32_t nowMs = millis();

  if (uwb.update()) {
    friendManager.observe(uwb.latestObservation(), nowMs);
  }

  friendManager.update(nowMs);
  applyFriendToAnimation(friendManager.nearestFriend(),
                         friendManager.heartState());

  for (int i = 0; i < 4; i++) {
    animation.update();
  }
}
