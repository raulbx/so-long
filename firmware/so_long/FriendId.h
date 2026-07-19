#pragma once

#include <Arduino.h>

// FriendId identifies the person a heart belongs to.
enum class FriendId : uint8_t {
  RAHUL,
  JENNIFER,
  MIKE,
  MAGGIE,
  JAMES,
  MAX,
};

constexpr uint8_t FRIEND_ID_COUNT = 6;
