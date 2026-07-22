#!/bin/sh
set -eu

test -f firmware/so_long/RangingEngine.h
test -f firmware/so_long/RangingEngine.cpp

rg "#include \"\\.\\./protocol/Protocol.h\"" firmware/so_long/RangingEngine.cpp >/dev/null
rg "#include \"Identity.h\"" firmware/so_long/RangingEngine.cpp >/dev/null
rg "PresenceObservation latestObservation\\(\\) const" firmware/so_long/RangingEngine.h >/dev/null
rg "#define POLL_MSG_PRESENCE_IDX 10" firmware/so_long/RangingEngine.cpp >/dev/null
rg "#define RESP_MSG_PRESENCE_IDX 18" firmware/so_long/RangingEngine.cpp >/dev/null
rg "#define POLL_MSG_FCS_IDX 13" firmware/so_long/RangingEngine.cpp >/dev/null
rg "#define RESP_MSG_FCS_IDX 21" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t tx_poll_msg\\[\\] = \\{0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0\\}" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t rx_resp_msg\\[\\] = \\{0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0\\}" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t rx_buffer\\[23\\]" firmware/so_long/RangingEngine.cpp >/dev/null
rg "Protocol::serialize" firmware/so_long/RangingEngine.cpp >/dev/null
rg "Protocol::deserialize" firmware/so_long/RangingEngine.cpp >/dev/null
rg "frame_len >= RESP_MSG_PRESENCE_IDX \\+ Protocol::PRESENCE_PACKET_SIZE" firmware/so_long/RangingEngine.cpp >/dev/null
rg "if \\(result.valid\\)" firmware/so_long/RangingEngine.cpp >/dev/null
rg "latestObservation_ = \\{" firmware/so_long/RangingEngine.cpp >/dev/null
rg "result.packet.nodeId" firmware/so_long/RangingEngine.cpp >/dev/null
rg "result.packet.friendId" firmware/so_long/RangingEngine.cpp >/dev/null

if rg "FriendId|PresencePacket|PresenceObservation|Protocol::|POLL_MSG_PRESENCE_IDX|RESP_MSG_PRESENCE_IDX|tx_poll_msg|rx_resp_msg" firmware/so_long/UWBManager.h firmware/so_long/UWBManager.cpp >/dev/null; then
  exit 1
fi

if rg "FriendId::JENNIFER|CURRENT_TEST_RESPONDER_NODE_ID" firmware/so_long/so_long.ino firmware/so_long/UWBManager.cpp firmware/so_long/UWBManager.h >/dev/null; then
  exit 1
fi

rg "friendManager\\.observe\\(ranging\\.latestObservation\\(\\), nowMs\\)" firmware/so_long/so_long.ino >/dev/null
rg "Protocol::serialize" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
rg "#define RESP_MSG_PRESENCE_IDX 18" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
rg "#define RESP_MSG_FCS_IDX 21" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
rg "static uint8_t rx_poll_msg\\[\\] = \\{0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0\\}" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
rg "static uint8_t tx_resp_msg\\[\\] = \\{0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0\\}" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
rg "static uint8_t rx_buffer\\[15\\]" firmware/prototypes/plain_range_tx/plain_range_tx.ino >/dev/null
