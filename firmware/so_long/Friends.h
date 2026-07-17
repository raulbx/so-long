#pragma once

#include <Arduino.h>
#include <FastLED.h>

enum class FriendId : uint8_t {
  RAHUL,
  JENNIFER,
  MIKE,
  MAGGIE,
  JAMES,
  MAX,
};

struct FriendInfo {
  FriendId id;
  const char* name;
  CRGB color;
};

static const FriendInfo FRIENDS[] = {
    {FriendId::RAHUL, "Rahul", CRGB(0, 180, 255)},
    {FriendId::JENNIFER, "Jennifer", CRGB::Red},
    {FriendId::MIKE, "Mike", CRGB(255, 120, 0)},
    {FriendId::MAGGIE, "Maggie", CRGB(120, 255, 80)},
    {FriendId::JAMES, "James", CRGB(160, 80, 255)},
    {FriendId::MAX, "Max", CRGB(255, 230, 80)},
};

static const size_t FRIEND_COUNT = sizeof(FRIENDS) / sizeof(FRIENDS[0]);
