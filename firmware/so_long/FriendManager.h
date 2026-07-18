#pragma once

#include <Arduino.h>

#include "FriendId.h"
#include "HeartState.h"

struct FriendObservation {
  FriendId id;
  float distanceM;
  uint32_t lastSeenMs;
  bool visible;
};

class FriendManager {
 public:
  void begin();
  void observe(FriendId id, float distanceM, uint32_t nowMs);
  void update(uint32_t nowMs);

  bool hasVisibleFriend() const;
  const FriendObservation* nearestFriend() const;
  HeartState heartState() const;

 private:
  static constexpr uint8_t kFriendSlotCount = 6;

  FriendObservation observations_[kFriendSlotCount];
  const FriendObservation* nearestFriend_;

  int8_t indexFor(FriendId id) const;
  HeartState heartStateForDistance(float distanceM) const;
};
