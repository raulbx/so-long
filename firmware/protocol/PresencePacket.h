#pragma once

#include "../so_long/FriendId.h"
#include "../so_long/NodeId.h"
#include "PacketVersion.h"

// PresencePacket represents the domain meaning of a heart announcing itself.
struct PresencePacket {
  ProtocolVersion version;
  NodeId nodeId;
  FriendId friendId;
};
