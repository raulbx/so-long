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

namespace ProtocolDetail {

constexpr uint8_t VERSION_INDEX = 0;
constexpr uint8_t NODE_INDEX = 1;
constexpr uint8_t FRIEND_INDEX = 2;

inline PresencePacket invalidPacket() {
  return {
      ProtocolVersion::V1,
      0,
      FriendId::RAHUL,
  };
}

}  // namespace ProtocolDetail

inline bool Protocol::serialize(const PresencePacket& packet, uint8_t* bytes,
                                size_t length) {
  if (bytes == nullptr || length != PRESENCE_PACKET_SIZE ||
      !isValidVersion(packet.version) || !isValidNodeId(packet.nodeId) ||
      !isValidFriendId(packet.friendId)) {
    return false;
  }

  bytes[ProtocolDetail::VERSION_INDEX] = static_cast<uint8_t>(packet.version);
  bytes[ProtocolDetail::NODE_INDEX] = static_cast<uint8_t>(packet.nodeId);
  bytes[ProtocolDetail::FRIEND_INDEX] = static_cast<uint8_t>(packet.friendId);

  return true;
}

inline PresencePacketResult Protocol::deserialize(const uint8_t* bytes,
                                                  size_t length) {
  if (bytes == nullptr || length != PRESENCE_PACKET_SIZE) {
    return {false, ProtocolDetail::invalidPacket()};
  }

  const ProtocolVersion version =
      static_cast<ProtocolVersion>(bytes[ProtocolDetail::VERSION_INDEX]);
  const NodeId nodeId = static_cast<NodeId>(bytes[ProtocolDetail::NODE_INDEX]);
  const FriendId friendId =
      static_cast<FriendId>(bytes[ProtocolDetail::FRIEND_INDEX]);

  if (!isValidVersion(version) || !isValidNodeId(nodeId) ||
      !isValidFriendId(friendId)) {
    return {false, ProtocolDetail::invalidPacket()};
  }

  return {
      true,
      {
          version,
          nodeId,
          friendId,
      },
  };
}

inline bool Protocol::isValidVersion(ProtocolVersion version) {
  return version == ProtocolVersion::V1;
}

inline bool Protocol::isValidNodeId(NodeId node) {
  return node != 0;
}

inline bool Protocol::isValidFriendId(FriendId friendId) {
  return static_cast<uint8_t>(friendId) < FRIEND_ID_COUNT;
}
