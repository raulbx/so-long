#pragma once

#include <stdint.h>

enum class HeartState : uint8_t {
  AMBIENT,
  FRIEND_DETECTED,
  FRIEND_NEAR,
  FRIEND_FOUND,
};
