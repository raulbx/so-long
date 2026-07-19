#!/bin/sh
set -eu

test -f firmware/so_long/FriendManager.h
test -f firmware/so_long/FriendManager.cpp
test -f firmware/so_long/PresenceObservation.h

rg "struct PresenceObservation" firmware/so_long/PresenceObservation.h >/dev/null
rg "NodeId nodeId" firmware/so_long/PresenceObservation.h >/dev/null
rg "FriendId friendId" firmware/so_long/PresenceObservation.h >/dev/null
rg "float distanceMeters" firmware/so_long/PresenceObservation.h >/dev/null

rg "struct FriendObservation" firmware/so_long/FriendManager.h >/dev/null
rg "FriendId id" firmware/so_long/FriendManager.h >/dev/null
rg "float distanceM" firmware/so_long/FriendManager.h >/dev/null
rg "uint32_t lastSeenMs" firmware/so_long/FriendManager.h >/dev/null
rg "bool visible" firmware/so_long/FriendManager.h >/dev/null

rg "void begin\\(\\)" firmware/so_long/FriendManager.h >/dev/null
rg "void observe\\(const PresenceObservation& observation, uint32_t nowMs\\)" firmware/so_long/FriendManager.h >/dev/null
rg "void update\\(uint32_t nowMs\\)" firmware/so_long/FriendManager.h >/dev/null
rg "bool hasVisibleFriend\\(\\) const" firmware/so_long/FriendManager.h >/dev/null
rg "const FriendObservation\\* nearestFriend\\(\\) const" firmware/so_long/FriendManager.h >/dev/null
rg "HeartState heartState\\(\\) const" firmware/so_long/FriendManager.h >/dev/null

rg "FRIEND_VISIBILITY_TIMEOUT_MS = 2000" firmware/so_long/Config.h >/dev/null
rg "distanceM < 1\\.0f" firmware/so_long/FriendManager.cpp >/dev/null
rg "HeartState::FRIEND_FOUND" firmware/so_long/FriendManager.cpp >/dev/null
rg "distanceM < 3\\.0f" firmware/so_long/FriendManager.cpp >/dev/null
rg "HeartState::FRIEND_NEAR" firmware/so_long/FriendManager.cpp >/dev/null
rg "HeartState::FRIEND_DETECTED" firmware/so_long/FriendManager.cpp >/dev/null
rg "HeartState::AMBIENT" firmware/so_long/FriendManager.cpp >/dev/null

if rg "dw3000|UWBManager|FastLED|AnimationEngine|CRGB|new |std::|vector|map" firmware/so_long/FriendManager.h firmware/so_long/FriendManager.cpp >/dev/null; then
  exit 1
fi

rg "#include \"FriendManager.h\"" firmware/so_long/so_long.ino >/dev/null
rg "FriendId::JENNIFER" firmware/so_long/so_long.ino >/dev/null
rg "uwb\\.latestDistanceMeters\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.observe\\(observation, nowMs\\)" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.update\\(nowMs\\)" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.nearestFriend\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.heartState\\(\\)" firmware/so_long/so_long.ino >/dev/null

if rg "heartStateForDistance|updateFriendTimeout|lastFriendRangeMs|lastFriendDistanceM|activeHeartState" firmware/so_long/so_long.ino >/dev/null; then
  exit 1
fi

if ! git diff --quiet -- firmware/prototypes/plain_range_tx.ino firmware/prototypes/working_range_rx_with_led.ino; then
  exit 1
fi
