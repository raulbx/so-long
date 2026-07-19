#pragma once

#include "FriendId.h"
#include "NodeId.h"

struct PresenceObservation {
  NodeId nodeId;
  FriendId friendId;
  float distanceMeters;
};
