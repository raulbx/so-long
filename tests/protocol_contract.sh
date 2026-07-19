#!/bin/sh
set -eu

test -f firmware/protocol/PacketVersion.h
test -f firmware/protocol/PresencePacket.h
test -f firmware/protocol/Protocol.h
test -f firmware/protocol/Protocol.cpp
test -f firmware/so_long/NodeId.h

cat >/tmp/protocol_contract.cpp <<'CPP'
#include <stdint.h>
#include <type_traits>

#include "PacketVersion.h"
#include "PresencePacket.h"
#include "Protocol.h"

static_assert(static_cast<uint8_t>(ProtocolVersion::V1) == 1);
static_assert(std::is_same<NodeId, uint8_t>::value);
static_assert(std::is_class<Protocol>::value);

int main() {
  PresencePacket packet{
      ProtocolVersion::V1,
      static_cast<NodeId>(1),
      FriendId::JENNIFER,
  };

  return packet.version == ProtocolVersion::V1 &&
                 packet.nodeId == static_cast<NodeId>(1) &&
                 packet.friendId == FriendId::JENNIFER
             ? 0
             : 1;
}
CPP

c++ -std=c++17 -Itests/stubs -Ifirmware/protocol -Ifirmware/so_long \
  /tmp/protocol_contract.cpp firmware/protocol/Protocol.cpp \
  -o /tmp/protocol_contract
/tmp/protocol_contract

if rg "#include \"\\.\\./protocol|#include \"Protocol" firmware/so_long/FriendManager.* firmware/so_long/AnimationEngine.* firmware/so_long/so_long.ino >/dev/null; then
  exit 1
fi

rg "Protocol::serialize" firmware/prototypes/plain_range_tx.ino >/dev/null
rg "LOCAL_FRIEND_ID = FriendId::JENNIFER" firmware/prototypes/plain_range_tx.ino >/dev/null

if ! git diff --quiet -- firmware/prototypes/working_range_rx_with_led.ino; then
  exit 1
fi
