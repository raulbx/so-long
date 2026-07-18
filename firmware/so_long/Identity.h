#pragma once

#include <Arduino.h>

#include "FriendId.h"
#include "NodeId.h"

// FriendId = who the heart belongs to.
constexpr FriendId MY_FRIEND = FriendId::RAHUL;

// NodeId = which physical radio/device this is.
constexpr NodeId MY_NODE_ID = 1;
