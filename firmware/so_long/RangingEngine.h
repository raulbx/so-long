#pragma once

#include <Arduino.h>

#include "PresenceObservation.h"
#include "UWBManager.h"

// RangingEngine owns the initiator-side SS-TWR exchange and translates it into
// domain observations.
class RangingEngine {
 public:
  explicit RangingEngine(UWBManager& uwb);

  bool update();
  float latestDistanceMeters() const;
  PresenceObservation latestObservation() const;

 private:
  UWBManager& uwb_;
  uint32_t lastRangeAttemptMs_ = 0;
  float latestDistanceMeters_ = -1.0f;
  float filteredDistanceMeters_ = -1.0f;
  PresenceObservation latestObservation_ = {0, FriendId::RAHUL, -1.0f};
};
