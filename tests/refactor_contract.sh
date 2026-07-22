#!/bin/sh
set -eu

test -f firmware/so_long/UWBManager.h
test -f firmware/so_long/UWBManager.cpp
test -f firmware/so_long/RangingEngine.h
test -f firmware/so_long/RangingEngine.cpp

rg "class UWBManager" firmware/so_long/UWBManager.h >/dev/null
rg "bool begin\\(\\)" firmware/so_long/UWBManager.h >/dev/null
rg "transmitAndExpectResponse" firmware/so_long/UWBManager.h >/dev/null
rg "transmitDelayed" firmware/so_long/UWBManager.h >/dev/null
rg "enableReceive" firmware/so_long/UWBManager.h >/dev/null
rg "readStatus\\(\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "hasReceivedFrame\\(uint32_t status\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "hasTransmitComplete\\(uint32_t status\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "txTimestampLo32\\(\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "rxTimestampLo32\\(\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "rxTimestamp64\\(\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "clockOffsetRatio\\(\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "setDelayedTransmitTime\\(uint32_t txTime\\) const" firmware/so_long/UWBManager.h >/dev/null
rg "txAntennaDelay\\(\\) const" firmware/so_long/UWBManager.h >/dev/null

rg "static dwt_config_t config" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_setrxaftertxdelay\\(POLL_TX_TO_RESP_RX_DLY_UUS\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_setrxtimeout\\(RESP_RX_TIMEOUT_UUS\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_writetxdata" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_rxenable\\(DWT_START_RX_IMMEDIATE\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_setdelayedtrxtime\\(txTime\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "get_rx_timestamp_u64" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_readtxtimestamplo32" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_readrxtimestamplo32" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_readclockoffset" firmware/so_long/UWBManager.cpp >/dev/null

if rg "FriendId|PresencePacket|PresenceObservation|Protocol::|tx_poll_msg|rx_resp_msg|resp_msg_get_ts|DWT_TIME_UNITS|SPEED_OF_LIGHT|distance =" firmware/so_long/UWBManager.h firmware/so_long/UWBManager.cpp >/dev/null; then
  exit 1
fi

rg "class RangingEngine" firmware/so_long/RangingEngine.h >/dev/null
rg "enum class RangingState" firmware/so_long/RangingEngine.h >/dev/null
rg "Listening" firmware/so_long/RangingEngine.h >/dev/null
rg "Initiating" firmware/so_long/RangingEngine.h >/dev/null
rg "WaitingForResponse" firmware/so_long/RangingEngine.h >/dev/null
rg "SendingResponse" firmware/so_long/RangingEngine.h >/dev/null
rg "RangingEngine\\(UWBManager& uwb, NodeId localNodeId, FriendId localFriendId\\)" firmware/so_long/RangingEngine.h >/dev/null
rg "bool update\\(\\)" firmware/so_long/RangingEngine.h >/dev/null
rg "float latestDistanceMeters\\(\\) const" firmware/so_long/RangingEngine.h >/dev/null
rg "PresenceObservation latestObservation\\(\\) const" firmware/so_long/RangingEngine.h >/dev/null

rg "#include \"UWBManager.h\"" firmware/so_long/RangingEngine.h >/dev/null
rg "#include \"\\.\\./protocol/Protocol.h\"" firmware/so_long/RangingEngine.cpp >/dev/null
if rg "#include \"Identity.h\"" firmware/so_long/RangingEngine.cpp >/dev/null; then
  exit 1
fi
rg "static uint8_t tx_poll_msg\\[\\]" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t rx_poll_msg\\[\\]" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t rx_resp_msg\\[\\]" firmware/so_long/RangingEngine.cpp >/dev/null
rg "static uint8_t tx_resp_msg\\[\\]" firmware/so_long/RangingEngine.cpp >/dev/null
rg "resp_msg_get_ts" firmware/so_long/RangingEngine.cpp >/dev/null
rg "resp_msg_set_ts" firmware/so_long/RangingEngine.cpp >/dev/null
rg "DWT_TIME_UNITS" firmware/so_long/RangingEngine.cpp >/dev/null
rg "SPEED_OF_LIGHT" firmware/so_long/RangingEngine.cpp >/dev/null
rg "POLL_RX_TO_RESP_TX_DLY_UUS 1200" firmware/so_long/RangingEngine.cpp >/dev/null
rg "UUS_TO_DWT_TIME" firmware/so_long/RangingEngine.cpp >/dev/null
rg "setDelayedTransmitTime" firmware/so_long/RangingEngine.cpp >/dev/null
rg "Protocol::serialize" firmware/so_long/RangingEngine.cpp >/dev/null
rg "Protocol::deserialize" firmware/so_long/RangingEngine.cpp >/dev/null
rg "0\\.7 \\* filteredDistanceMeters_ \\+ 0\\.3 \\* distance" firmware/so_long/RangingEngine.cpp >/dev/null

rg "#include \"UWBManager.h\"" firmware/so_long/so_long.ino >/dev/null
rg "#include \"RangingEngine.h\"" firmware/so_long/so_long.ino >/dev/null
rg "RangingEngine ranging\\(uwb, MY_NODE_ID, MY_FRIEND\\)" firmware/so_long/so_long.ino >/dev/null
rg "uwb.begin\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "ranging.update\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "ranging.latestObservation\\(\\)" firmware/so_long/so_long.ino >/dev/null

rg "setCometSpeedMs" firmware/so_long/AnimationEngine.h >/dev/null
if rg "delay\\(" firmware/so_long/AnimationEngine.cpp firmware/so_long/AnimationEngine.h >/dev/null; then
  exit 1
fi

test -f firmware/prototypes/plain_range_tx/plain_range_tx.ino
