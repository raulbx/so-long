#include "Protocol.h"

namespace {

constexpr uint8_t kVersionIndex = 0;
constexpr uint8_t kNodeIndex = 1;
constexpr uint8_t kFriendIndex = 2;

PresencePacket invalidPacket() {
  return {
      ProtocolVersion::V1,
      0,
      FriendId::RAHUL,
  };
}

}  // namespace

bool Protocol::serialize(const PresencePacket& packet, uint8_t* bytes,
                         size_t length) {
  if (bytes == nullptr || length != PRESENCE_PACKET_SIZE ||
      !isValidVersion(packet.version) || !isValidNodeId(packet.node) ||
      !isValidFriendId(packet.friendId)) {
    return false;
  }

  bytes[kVersionIndex] = static_cast<uint8_t>(packet.version);
  bytes[kNodeIndex] = static_cast<uint8_t>(packet.node);
  bytes[kFriendIndex] = static_cast<uint8_t>(packet.friendId);

  return true;
}

PresencePacketResult Protocol::deserialize(const uint8_t* bytes, size_t length) {
  if (bytes == nullptr || length != PRESENCE_PACKET_SIZE) {
    return {false, invalidPacket()};
  }

  const ProtocolVersion version =
      static_cast<ProtocolVersion>(bytes[kVersionIndex]);
  const NodeId node = static_cast<NodeId>(bytes[kNodeIndex]);
  const FriendId friendId = static_cast<FriendId>(bytes[kFriendIndex]);

  if (!isValidVersion(version) || !isValidNodeId(node) ||
      !isValidFriendId(friendId)) {
    return {false, invalidPacket()};
  }

  return {
      true,
      {
          version,
          node,
          friendId,
      },
  };
}

bool Protocol::isValidVersion(ProtocolVersion version) {
  return version == ProtocolVersion::V1;
}

bool Protocol::isValidNodeId(NodeId node) {
  return node != 0;
}

bool Protocol::isValidFriendId(FriendId friendId) {
  return static_cast<uint8_t>(friendId) < FRIEND_ID_COUNT;
}
