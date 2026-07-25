#pragma once

#ifndef SO_LONG_FRIEND_ID_H
#define SO_LONG_FRIEND_ID_H

#include <stdint.h>

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

#endif  // SO_LONG_FRIEND_ID_H
