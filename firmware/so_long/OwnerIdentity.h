#pragma once

#include "FriendId.h"
#include "Friends.h"
#include "Identity.h"

inline const FriendInfo* friendInfoFor(FriendId id) {
  for (size_t i = 0; i < FRIEND_COUNT; i++) {
    if (FRIENDS[i].id == id) {
      return &FRIENDS[i];
    }
  }

  return nullptr;
}

inline const FriendInfo* localOwnerInfo() {
  return friendInfoFor(MY_FRIEND);
}
