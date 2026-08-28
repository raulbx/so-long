#pragma once

#include <stdint.h>

#include "DomainColor.h"
#include "Emotion.h"

struct EmotionalState {
  Emotion emotion;
  Color ownerColor;
  bool friendPresent;
  Color friendColor;
  uint8_t intensity;
  uint8_t effectSize;
  uint16_t motionIntervalMs;
};
