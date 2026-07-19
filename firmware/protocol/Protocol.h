#pragma once

#include <Arduino.h>

#include "PresencePacket.h"

struct PresencePacketResult {
  bool valid;
  PresencePacket packet;
};

// Protocol translates the v1 PresencePacket domain object to and from bytes.
class Protocol {
 public:
  static constexpr size_t PRESENCE_PACKET_SIZE = 3;

  static bool serialize(const PresencePacket& packet, uint8_t* bytes,
                        size_t length);
  static PresencePacketResult deserialize(const uint8_t* bytes, size_t length);

 private:
  static bool isValidVersion(ProtocolVersion version);
  static bool isValidNodeId(NodeId node);
  static bool isValidFriendId(FriendId friendId);
};
