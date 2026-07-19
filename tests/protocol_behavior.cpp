#include <assert.h>
#include <stdint.h>

#include "Protocol.h"

void assertPacketEquals(const PresencePacket& packet, ProtocolVersion version,
                        NodeId node, FriendId friendId) {
  assert(packet.version == version);
  assert(packet.nodeId == node);
  assert(packet.friendId == friendId);
}

int main() {
  const PresencePacket packet{
      ProtocolVersion::V1,
      static_cast<NodeId>(1),
      FriendId::JENNIFER,
  };

  uint8_t bytes[Protocol::PRESENCE_PACKET_SIZE] = {0, 0, 0};
  assert(Protocol::serialize(packet, bytes, sizeof(bytes)));
  assert(bytes[0] == 1);
  assert(bytes[1] == 1);
  assert(bytes[2] == static_cast<uint8_t>(FriendId::JENNIFER));

  const PresencePacketResult result =
      Protocol::deserialize(bytes, sizeof(bytes));
  assert(result.valid);
  assertPacketEquals(result.packet, ProtocolVersion::V1, 1,
                     FriendId::JENNIFER);

  uint8_t roundTripBytes[Protocol::PRESENCE_PACKET_SIZE] = {0, 0, 0};
  assert(Protocol::serialize(result.packet, roundTripBytes,
                             sizeof(roundTripBytes)));
  assert(roundTripBytes[0] == bytes[0]);
  assert(roundTripBytes[1] == bytes[1]);
  assert(roundTripBytes[2] == bytes[2]);

  uint8_t pollFrame[15] = {};
  const size_t pollPresenceOffset = 10;
  assert(Protocol::serialize(packet, &pollFrame[pollPresenceOffset],
                             Protocol::PRESENCE_PACKET_SIZE));
  assert(pollFrame[13] == 0);
  assert(pollFrame[14] == 0);

  uint8_t responseFrame[23] = {};
  const size_t responsePresenceOffset = 18;
  assert(Protocol::serialize(packet, &responseFrame[responsePresenceOffset],
                             Protocol::PRESENCE_PACKET_SIZE));
  const PresencePacketResult transportedResult = Protocol::deserialize(
      &responseFrame[responsePresenceOffset], Protocol::PRESENCE_PACKET_SIZE);
  assert(transportedResult.valid);
  assertPacketEquals(transportedResult.packet, ProtocolVersion::V1, 1,
                     FriendId::JENNIFER);
  assert(responseFrame[21] == 0);
  assert(responseFrame[22] == 0);

  const PresencePacket maxFriendPacket{
      ProtocolVersion::V1,
      static_cast<NodeId>(2),
      FriendId::MAX,
  };
  assert(Protocol::serialize(maxFriendPacket, bytes, sizeof(bytes)));
  const PresencePacketResult maxFriendResult =
      Protocol::deserialize(bytes, sizeof(bytes));
  assert(maxFriendResult.valid);
  assertPacketEquals(maxFriendResult.packet, ProtocolVersion::V1, 2,
                     FriendId::MAX);

  uint8_t tooShort[2] = {1, 1};
  assert(!Protocol::deserialize(tooShort, sizeof(tooShort)).valid);
  assert(!Protocol::serialize(packet, tooShort, sizeof(tooShort)));

  uint8_t tooLong[4] = {1, 1, static_cast<uint8_t>(FriendId::JENNIFER), 0};
  assert(!Protocol::deserialize(tooLong, sizeof(tooLong)).valid);
  assert(!Protocol::serialize(packet, tooLong, sizeof(tooLong)));

  uint8_t unsupportedVersion[Protocol::PRESENCE_PACKET_SIZE] = {
      2,
      1,
      static_cast<uint8_t>(FriendId::JENNIFER),
  };
  assert(!Protocol::deserialize(unsupportedVersion,
                                sizeof(unsupportedVersion))
              .valid);

  uint8_t invalidNode[Protocol::PRESENCE_PACKET_SIZE] = {
      1,
      0,
      static_cast<uint8_t>(FriendId::JENNIFER),
  };
  assert(!Protocol::deserialize(invalidNode, sizeof(invalidNode)).valid);

  uint8_t invalidFriend[Protocol::PRESENCE_PACKET_SIZE] = {
      1,
      1,
      255,
  };
  assert(!Protocol::deserialize(invalidFriend, sizeof(invalidFriend)).valid);

  const PresencePacket invalidVersionPacket{
      static_cast<ProtocolVersion>(2),
      static_cast<NodeId>(1),
      FriendId::JENNIFER,
  };
  assert(!Protocol::serialize(invalidVersionPacket, bytes, sizeof(bytes)));

  const PresencePacket invalidNodePacket{
      ProtocolVersion::V1,
      static_cast<NodeId>(0),
      FriendId::JENNIFER,
  };
  assert(!Protocol::serialize(invalidNodePacket, bytes, sizeof(bytes)));

  const PresencePacket invalidFriendPacket{
      ProtocolVersion::V1,
      static_cast<NodeId>(1),
      static_cast<FriendId>(FRIEND_ID_COUNT),
  };
  assert(!Protocol::serialize(invalidFriendPacket, bytes, sizeof(bytes)));

  assert(!Protocol::serialize(packet, nullptr, sizeof(bytes)));
  assert(!Protocol::deserialize(nullptr, sizeof(bytes)).valid);

  return 0;
}
