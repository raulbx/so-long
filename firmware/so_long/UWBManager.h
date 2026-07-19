#pragma once

#include <Arduino.h>

#include "PresenceObservation.h"

class UWBManager {
 public:
  bool begin();
  bool update();
  float latestDistanceMeters() const;
  PresenceObservation latestObservation() const;

 private:
  float latestDistanceMeters_ = -1.0f;
  float filteredDistanceMeters_ = -1.0f;
  PresenceObservation latestObservation_ = {0, FriendId::RAHUL, -1.0f};
};
