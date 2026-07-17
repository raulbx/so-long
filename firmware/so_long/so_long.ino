#include <Arduino.h>
#include <FastLED.h>

#include "AnimationEngine.h"
#include "Config.h"
#include "Friends.h"
#include "UWBManager.h"

CRGB leds[SoLongConfig::LED_COUNT];
AnimationEngine animation(leds, SoLongConfig::LED_COUNT);
UWBManager uwb;

FriendId activeFriendId = FriendId::JENNIFER;
HeartState activeHeartState = HeartState::AMBIENT;
uint32_t lastFriendRangeMs = 0;
float lastFriendDistanceM = 0.0f;

const FriendInfo* findFriend(FriendId id) {
  for (size_t i = 0; i < FRIEND_COUNT; i++) {
    if (FRIENDS[i].id == id) {
      return &FRIENDS[i];
    }
  }
  return nullptr;
}

HeartState heartStateForDistance(float distanceM) {
  if (distanceM < 1.0f) {
    return HeartState::FRIEND_FOUND;
  }
  if (distanceM < 3.0f) {
    return HeartState::FRIEND_NEAR;
  }
  return HeartState::FRIEND_DETECTED;
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

void observeFriend(FriendId friendId, float distanceM) {
  activeFriendId = friendId;
  lastFriendDistanceM = distanceM;
  lastFriendRangeMs = millis();

  const FriendInfo* friendInfo = findFriend(friendId);
  if (friendInfo != nullptr) {
    animation.setFriendColor(friendInfo->color);
  }

  activeHeartState = heartStateForDistance(distanceM);
  animation.setHeartState(activeHeartState);
  animation.setCometSpeedMs(cometSpeedForDistance(distanceM));
}

void updateFriendTimeout() {
  if (activeHeartState == HeartState::AMBIENT) {
    return;
  }

  if (millis() - lastFriendRangeMs > SoLongConfig::FRIEND_TIMEOUT_MS) {
    activeHeartState = HeartState::AMBIENT;
    animation.setHeartState(activeHeartState);
  }
}

void setup() {
  FastLED.addLeds<WS2811, SoLongConfig::LED_DATA_PIN,
                  SoLongConfig::LED_COLOR_ORDER>(
      leds, SoLongConfig::LED_COUNT);

  animation.begin();
  animation.setCometSpeedMs(90);
  observeFriend(FriendId::JENNIFER, 100.0f);

  uwb.begin();
}

void loop() {
  if (uwb.update()) {
    observeFriend(FriendId::JENNIFER, uwb.latestDistanceMeters());
  }
  for (int i = 0; i < 4; i++) {
    animation.update();
  }
}
