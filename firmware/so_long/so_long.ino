#include <Arduino.h>
#include <FastLED.h>

#include "AnimationEngine.h"
#include "Config.h"
#include "Debug.h"
#include "FriendManager.h"
#include "Friends.h"
#include "Identity.h"
#include "OwnerIdentity.h"
#include "RangingEngine.h"
#include "UWBManager.h"

CRGB leds[SoLongConfig::LED_COUNT];
AnimationEngine animation(leds, SoLongConfig::LED_COUNT);
FriendManager friendManager;
UWBManager uwb;
RangingEngine ranging(uwb, MY_NODE_ID, MY_FRIEND);

FriendId activeFriendId = MY_FRIEND;

void applyFriendToAnimation(const FriendObservation* observation,
                            HeartState heartState) {
  if (observation == nullptr) {
    animation.setHeartState(heartState);
    return;
  }

  activeFriendId = observation->id;
  const FriendInfo* friendInfo = friendInfoFor(observation->id);
  if (friendInfo != nullptr) {
    animation.setFriendColor(friendInfo->color);
  }

  animation.setHeartState(heartState);
  animation.setFriendDistanceMeters(observation->distanceM);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Application starting");
  Serial.print("Node ID: ");
  Serial.println(static_cast<uint8_t>(MY_NODE_ID));
  Serial.print("Friend ID: ");
  Serial.println(static_cast<uint8_t>(MY_FRIEND));
  
  FastLED.addLeds<WS2811, SoLongConfig::LED_DATA_PIN,
                  SO_LONG_LED_COLOR_ORDER>(
      leds, SoLongConfig::LED_COUNT);

  animation.begin();
  const FriendInfo* ownerInfo = localOwnerInfo();
  if (ownerInfo != nullptr) {
    animation.setOwnerColor(ownerInfo->color);
  }
  animation.setCometSpeedMs(SoLongConfig::COMET_SLOW_MS);
  friendManager.begin();
  bool ok = uwb.begin();
  Serial.println(ok);
}

void loop() {
  const uint32_t nowMs = millis();

  if (ranging.update()) {
    friendManager.observe(ranging.latestObservation(), nowMs);
  }

  friendManager.update(nowMs);
  applyFriendToAnimation(friendManager.nearestFriend(),
                         friendManager.heartState());

  for (int i = 0; i < 4; i++) {
    animation.update();
  }
}
