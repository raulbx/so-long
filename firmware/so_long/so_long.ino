#include <Arduino.h>
#include <FastLED.h>

#include "AnimationEngine.h"
#include "Config.h"
#include "Debug.h"
#include "EmotionalStateEngine.h"
#include "FriendManager.h"
#include "Friends.h"
#include "Identity.h"
#include "OwnerIdentity.h"
#include "RangingEngine.h"
#include "UWBManager.h"

CRGB leds[SoLongConfig::LED_COUNT];
AnimationEngine animation(leds, SoLongConfig::LED_COUNT);
EmotionalStateEngine emotionalState;
FriendManager friendManager;
UWBManager uwb;
RangingEngine ranging(uwb, MY_NODE_ID, MY_FRIEND);

void updateEmotionalState(const FriendObservation* observation,
                          uint32_t nowMs) {
  Color friendColor = SoLongColors::Black;
  if (observation == nullptr) {
    emotionalState.update(nullptr, friendColor, nowMs);
  } else {
    const FriendInfo* friendInfo = friendInfoFor(observation->id);
    if (friendInfo != nullptr) {
      friendColor = friendInfo->color;
    }
    emotionalState.update(observation, friendColor, nowMs);
  }

  animation.setEmotionalState(emotionalState.currentState());
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
    emotionalState.begin(ownerInfo->color);
  } else {
    emotionalState.begin(SoLongColors::DeepSkyBlue);
  }
  animation.setEmotionalState(emotionalState.currentState());
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
  updateEmotionalState(friendManager.nearestFriend(), nowMs);

  for (int i = 0; i < 4; i++) {
    animation.update();
  }
}
