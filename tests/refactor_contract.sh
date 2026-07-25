#!/bin/sh
set -eu

test -f firmware/so_long/UWBManager.h
test -f firmware/so_long/UWBManager.cpp
test -f firmware/so_long/RangingEngine.h
test -f firmware/so_long/RangingEngine.cpp
test -f firmware/so_long/RangingSchedule.h
test -f firmware/so_long/Debug.h
test -f firmware/so_long/OwnerIdentity.h
test -f firmware/so_long/DomainColor.h
test -f firmware/so_long/Emotion.h
test -f firmware/so_long/EmotionalState.h
test -f firmware/so_long/EmotionalStateEngine.h
test -f firmware/so_long/EmotionalStateEngine.cpp

rg "class UWBManager" firmware/so_long/UWBManager.h >/dev/null
rg "bool begin\\(\\)" firmware/so_long/UWBManager.h >/dev/null
rg "int transmitAndExpectResponse" firmware/so_long/UWBManager.h >/dev/null
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
rg "dwt_forcetrxoff\\(\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_writetxdata" firmware/so_long/UWBManager.cpp >/dev/null
rg "const int result = dwt_starttx" firmware/so_long/UWBManager.cpp >/dev/null
rg "return result" firmware/so_long/UWBManager.cpp >/dev/null
rg "SO_LONG_UWB_DEBUG" firmware/so_long/Debug.h firmware/so_long/UWBManager.cpp firmware/so_long/RangingEngine.cpp >/dev/null
rg "SO_LONG_APP_DEBUG" firmware/so_long/Debug.h >/dev/null
rg "#include \"Debug.h\"" firmware/so_long/so_long.ino >/dev/null
if rg "accepted observation|selected active friend|FastLED\\.show animation|animationNameForState|skipped initiation slot|entered WaitingForResponse|SYS_STATUS_TXFRS observed|RXFCG observed|transmitAndExpectResponse returned" firmware/so_long >/dev/null; then
  exit 1
fi
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
rg "isInitiationSlotForNode" firmware/so_long/RangingSchedule.h >/dev/null
rg "missedInitiationSkipSlots" firmware/so_long/RangingSchedule.h >/dev/null
rg "RANGING_SLOT_MS = 100" firmware/so_long/RangingSchedule.h >/dev/null

rg "#include \"UWBManager.h\"" firmware/so_long/RangingEngine.h >/dev/null
rg "#include \"RangingSchedule.h\"" firmware/so_long/RangingEngine.cpp >/dev/null
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
rg "isInitiationSlotForNode\\(localNodeId_, nowMs\\)" firmware/so_long/RangingEngine.cpp >/dev/null
rg "slotsToSkip_" firmware/so_long/RangingEngine.h firmware/so_long/RangingEngine.cpp >/dev/null
rg "missedInitiationSkipSlots\\(localNodeId_\\)" firmware/so_long/RangingEngine.cpp >/dev/null
rg "0\\.7 \\* filteredDistanceMeters_ \\+ 0\\.3 \\* distance" firmware/so_long/RangingEngine.cpp >/dev/null

rg "#include \"UWBManager.h\"" firmware/so_long/so_long.ino >/dev/null
rg "#include \"RangingEngine.h\"" firmware/so_long/so_long.ino >/dev/null
rg "RangingEngine ranging\\(uwb, MY_NODE_ID, MY_FRIEND\\)" firmware/so_long/so_long.ino >/dev/null
rg "uwb.begin\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "ranging.update\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "ranging.latestObservation\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "EmotionalStateEngine emotionalState" firmware/so_long/so_long.ino >/dev/null
rg "updateEmotionalState\\(friendManager\\.nearestFriend\\(\\), nowMs\\)" firmware/so_long/so_long.ino >/dev/null
rg "emotionalState\\.update\\(" firmware/so_long/so_long.ino >/dev/null
rg "animation\\.setEmotionalState\\(emotionalState\\.currentState\\(\\)\\)" firmware/so_long/so_long.ino >/dev/null

rg "setEmotionalState\\(EmotionalState state\\)" firmware/so_long/AnimationEngine.h firmware/so_long/AnimationEngine.cpp >/dev/null
rg "renderOwnerBreathBackground" firmware/so_long/AnimationEngine.h firmware/so_long/AnimationEngine.cpp >/dev/null
rg "toCrgb\\(Color color\\)" firmware/so_long/AnimationEngine.h firmware/so_long/AnimationEngine.cpp >/dev/null
if rg "FriendId|Identity|FriendInfo|FRIENDS|MY_FRIEND" firmware/so_long/AnimationEngine.h firmware/so_long/AnimationEngine.cpp >/dev/null; then
  exit 1
fi
if rg "HeartState" firmware/so_long/AnimationEngine.h firmware/so_long/AnimationEngine.cpp >/dev/null; then
  exit 1
fi
if rg "delay\\(" firmware/so_long/AnimationEngine.cpp firmware/so_long/AnimationEngine.h >/dev/null; then
  exit 1
fi

if rg "Arduino|FastLED|CRGB|DW3000|dw3000|UWBManager|RangingEngine" firmware/so_long/DomainColor.h firmware/so_long/Emotion.h firmware/so_long/EmotionalState.h firmware/so_long/FriendId.h firmware/so_long/NodeId.h firmware/so_long/HeartState.h >/dev/null; then
  exit 1
fi

if rg "FastLED|CRGB|DW3000|dw3000|UWBManager|RangingEngine" firmware/so_long/EmotionalStateEngine.h firmware/so_long/EmotionalStateEngine.cpp >/dev/null; then
  exit 1
fi

if rg "FastLED|CRGB" firmware/so_long/Friends.h >/dev/null; then
  exit 1
fi

test -f firmware/prototypes/plain_range_tx/plain_range_tx.ino
