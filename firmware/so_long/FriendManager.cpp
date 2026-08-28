#include "FriendManager.h"

#include "Config.h"

void FriendManager::begin() {
  for (uint8_t i = 0; i < kFriendSlotCount; i++) {
    observations_[i] = {
        static_cast<FriendId>(i),
        0.0f,
        0,
        false,
    };
  }

  nearestFriend_ = nullptr;
}

void FriendManager::observe(const PresenceObservation& observation,
                            uint32_t nowMs) {
  const int8_t index = indexFor(observation.friendId);
  if (index < 0) {
    return;
  }

  observations_[index].distanceM = observation.distanceMeters;
  observations_[index].lastSeenMs = nowMs;
  observations_[index].visible = true;
}

void FriendManager::update(uint32_t nowMs) {
  nearestFriend_ = nullptr;

  for (uint8_t i = 0; i < kFriendSlotCount; i++) {
    FriendObservation& observation = observations_[i];

    if (observation.visible &&
        nowMs - observation.lastSeenMs >
            SoLongConfig::FRIEND_VISIBILITY_TIMEOUT_MS) {
      observation.visible = false;
    }

    if (!observation.visible) {
      continue;
    }

    if (nearestFriend_ == nullptr ||
        observation.distanceM < nearestFriend_->distanceM) {
      nearestFriend_ = &observation;
    }
  }
}

bool FriendManager::hasVisibleFriend() const {
  return nearestFriend_ != nullptr;
}

const FriendObservation* FriendManager::nearestFriend() const {
  return nearestFriend_;
}

int8_t FriendManager::indexFor(FriendId id) const {
  const uint8_t index = static_cast<uint8_t>(id);
  if (index >= kFriendSlotCount) {
    return -1;
  }

  return index;
}
