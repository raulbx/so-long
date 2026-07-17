#!/bin/sh
set -eu

test -f firmware/so_long/Identity.h

rg "constexpr FriendId MY_FRIEND = FriendId::RAHUL" firmware/so_long/Identity.h >/dev/null
rg "constexpr uint8_t MY_NODE_ID = 1" firmware/so_long/Identity.h >/dev/null
rg "FriendId = who the heart belongs to" firmware/so_long/Identity.h >/dev/null
rg "NodeId = which physical radio/device this is" firmware/so_long/Identity.h >/dev/null

rg "#include \"Identity.h\"" firmware/so_long/so_long.ino >/dev/null
rg "FriendId activeFriendId = MY_FRIEND" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.observe\\(FriendId::JENNIFER, uwb\\.latestDistanceMeters\\(\\), nowMs\\)" firmware/so_long/so_long.ino >/dev/null

if rg "MY_NODE_ID" firmware/so_long/UWBManager.cpp firmware/so_long/UWBManager.h >/dev/null; then
  exit 1
fi

if ! git diff --quiet -- firmware/prototypes/plain_range_tx.ino firmware/prototypes/working_range_rx_with_led.ino; then
  exit 1
fi
