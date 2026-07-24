#!/bin/sh
set -eu

test -f firmware/so_long/Identity.h
test -f firmware/so_long/NodeId.h

rg "SO_LONG_BOARD_ID" firmware/so_long/Identity.h >/dev/null
rg "#if SO_LONG_BOARD_ID == 1" firmware/so_long/Identity.h >/dev/null
rg "FriendId::RAHUL" firmware/so_long/Identity.h >/dev/null
rg "#elif SO_LONG_BOARD_ID == 2" firmware/so_long/Identity.h >/dev/null
rg "FriendId::JENNIFER" firmware/so_long/Identity.h >/dev/null
rg "using NodeId = uint8_t" firmware/so_long/NodeId.h >/dev/null
rg "FriendId = who the heart belongs to" firmware/so_long/Identity.h >/dev/null
rg "NodeId = which physical radio/device this is" firmware/so_long/Identity.h >/dev/null

rg "#include \"Identity.h\"" firmware/so_long/so_long.ino >/dev/null
rg "FriendId activeFriendId = MY_FRIEND" firmware/so_long/so_long.ino >/dev/null
rg "RangingEngine ranging\\(uwb, MY_NODE_ID, MY_FRIEND\\)" firmware/so_long/so_long.ino >/dev/null
rg "friendManager\\.observe\\(ranging\\.latestObservation\\(\\), nowMs\\)" firmware/so_long/so_long.ino >/dev/null

if rg "MY_NODE_ID|MY_FRIEND|#include \"Identity.h\"" firmware/so_long/RangingEngine.cpp firmware/so_long/RangingEngine.h >/dev/null; then
  exit 1
fi

c++ -std=c++17 -DSO_LONG_BOARD_ID=1 -Itests/stubs -Ifirmware/so_long \
  tests/ranging_schedule_behavior.cpp -o /tmp/ranging_schedule_board_a
/tmp/ranging_schedule_board_a

c++ -std=c++17 -DSO_LONG_BOARD_ID=2 -Itests/stubs -Ifirmware/so_long \
  tests/ranging_schedule_behavior.cpp -o /tmp/ranging_schedule_board_b
/tmp/ranging_schedule_board_b

if ! git diff --quiet -- firmware/prototypes/working_range_rx_with_led.ino; then
  exit 1
fi
