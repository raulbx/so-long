#!/bin/sh
set -eu

test -f firmware/so_long/UWBManager.h
test -f firmware/so_long/UWBManager.cpp

rg "class UWBManager" firmware/so_long/UWBManager.h >/dev/null
rg "bool begin\\(\\)" firmware/so_long/UWBManager.h >/dev/null
rg "bool update\\(\\)" firmware/so_long/UWBManager.h >/dev/null
rg "float latestDistanceMeters\\(\\) const" firmware/so_long/UWBManager.h >/dev/null

rg "static dwt_config_t config" firmware/so_long/UWBManager.cpp >/dev/null
rg "static uint8_t tx_poll_msg\\[\\]" firmware/so_long/UWBManager.cpp >/dev/null
rg "static uint8_t rx_resp_msg\\[\\]" firmware/so_long/UWBManager.cpp >/dev/null
rg "resp_msg_get_ts" firmware/so_long/UWBManager.cpp >/dev/null
rg "DWT_TIME_UNITS" firmware/so_long/UWBManager.cpp >/dev/null
rg "SPEED_OF_LIGHT" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_setrxaftertxdelay\\(POLL_TX_TO_RESP_RX_DLY_UUS\\)" firmware/so_long/UWBManager.cpp >/dev/null
rg "dwt_setrxtimeout\\(RESP_RX_TIMEOUT_UUS\\)" firmware/so_long/UWBManager.cpp >/dev/null

rg "#include \"UWBManager.h\"" firmware/so_long/so_long.ino >/dev/null
rg "uwb.begin\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "uwb.update\\(\\)" firmware/so_long/so_long.ino >/dev/null
rg "uwb.latestObservation\\(\\)" firmware/so_long/so_long.ino >/dev/null

rg "setCometSpeedMs" firmware/so_long/AnimationEngine.h >/dev/null
if rg "delay\\(" firmware/so_long/AnimationEngine.cpp firmware/so_long/AnimationEngine.h >/dev/null; then
  exit 1
fi

test -f firmware/prototypes/plain_range_tx.ino
