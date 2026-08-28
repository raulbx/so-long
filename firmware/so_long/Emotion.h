#pragma once

#include <stdint.h>

enum class Emotion : uint8_t {
  IDLE,
  PRESENT,
  APPROACHING,
  CURIOUS,
  REUNITED,
  DEPARTING,
};
