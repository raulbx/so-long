#pragma once

#include <Arduino.h>

#include "FriendId.h"
#include "NodeId.h"

// -----------------------------------------------------------------------------
// Device Configuration
//
// Select a physical board at compile time.
// Arduino CLI examples:
//   Board A: --build-property compiler.cpp.extra_flags="-DSO_LONG_BOARD_ID=1"
//   Board B: --build-property compiler.cpp.extra_flags="-DSO_LONG_BOARD_ID=2"
//
// Board A:
//   MY_NODE_ID = 1
//   MY_FRIEND  = FriendId::RAHUL
//
// Board B:
//   MY_NODE_ID = 2
//   MY_FRIEND  = FriendId::JENNIFER
// -----------------------------------------------------------------------------

#ifndef SO_LONG_BOARD_ID
#define SO_LONG_BOARD_ID 1
#endif

// FriendId = who the heart belongs to.
// NodeId = which physical radio/device this is.
#if SO_LONG_BOARD_ID == 1
constexpr NodeId MY_NODE_ID = 1;
constexpr FriendId MY_FRIEND = FriendId::RAHUL;
#elif SO_LONG_BOARD_ID == 2
constexpr NodeId MY_NODE_ID = 2;
constexpr FriendId MY_FRIEND = FriendId::JENNIFER;
#else
#error "Unsupported SO_LONG_BOARD_ID. Use 1 for Board A or 2 for Board B."
#endif
