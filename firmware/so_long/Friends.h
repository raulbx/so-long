#pragma once

#include <stddef.h>

#include "DomainColor.h"
#include "FriendId.h"

struct FriendInfo {
  FriendId id;
  const char* name;
  Color color;
};

static const FriendInfo FRIENDS[] = {
  { FriendId::RAHUL, "Rahul", SoLongColors::Blue },
  { FriendId::JENNIFER, "Jennifer", SoLongColors::Red },
  { FriendId::MIKE, "Mike", SoLongColors::Orange },
  { FriendId::MAGGIE, "Maggie", SoLongColors::Green },
  { FriendId::JAMES, "James", SoLongColors::Purple },
  { FriendId::MAX, "Max", SoLongColors::Yellow },
};

static const size_t FRIEND_COUNT = sizeof(FRIENDS) / sizeof(FRIENDS[0]);
