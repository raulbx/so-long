#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "FriendId.h"

struct FriendInfo {
  FriendId id;
  const char* name;
  CRGB color;
};

static const FriendInfo FRIENDS[] = {
  { FriendId::RAHUL, "Rahul", CRGB::Blue },
  { FriendId::JENNIFER, "Jennifer", CRGB::Red },
  { FriendId::MIKE, "Mike", CRGB::Orange },
  { FriendId::MAGGIE, "Maggie", CRGB::Green },
  { FriendId::JAMES, "James", CRGB::Purple },
  { FriendId::MAX, "Max", CRGB::Yellow },
};

static const size_t FRIEND_COUNT = sizeof(FRIENDS) / sizeof(FRIENDS[0]);
