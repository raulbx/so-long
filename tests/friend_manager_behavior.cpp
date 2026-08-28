#include <assert.h>
#include "FriendManager.h"

PresenceObservation observation(NodeId nodeId, FriendId friendId,
                                float distanceMeters) {
  return {
      nodeId,
      friendId,
      distanceMeters,
  };
}

int main() {
  FriendManager friends;
  friends.begin();

  friends.update(0);
  assert(!friends.hasVisibleFriend());
  assert(friends.nearestFriend() == nullptr);

  friends.observe(observation(2, FriendId::JENNIFER, 4.0f), 100);
  friends.update(100);
  assert(friends.hasVisibleFriend());
  assert(friends.nearestFriend()->id == FriendId::JENNIFER);
  assert(friends.nearestFriend()->distanceM == 4.0f);

  friends.observe(observation(3, FriendId::MIKE, 2.0f), 150);
  friends.update(150);
  assert(friends.nearestFriend()->id == FriendId::MIKE);
  assert(friends.nearestFriend()->distanceM == 2.0f);

  friends.observe(observation(4, FriendId::MAX, 0.5f), 200);
  friends.update(200);
  assert(friends.nearestFriend()->id == FriendId::MAX);
  assert(friends.nearestFriend()->distanceM == 0.5f);

  friends.update(2401);
  assert(!friends.hasVisibleFriend());
  assert(friends.nearestFriend() == nullptr);

  friends.observe(observation(1, FriendId::RAHUL, 1.0f), 2500);
  friends.update(2500);
  assert(friends.nearestFriend()->id == FriendId::RAHUL);
  assert(friends.nearestFriend()->distanceM == 1.0f);

  FriendManager rahulFriends;
  rahulFriends.begin();
  rahulFriends.observe(observation(1, FriendId::RAHUL, 0.93f), 2600);
  rahulFriends.update(2600);
  assert(rahulFriends.hasVisibleFriend());
  assert(rahulFriends.nearestFriend()->id == FriendId::RAHUL);
  assert(rahulFriends.nearestFriend()->distanceM == 0.93f);

  friends.observe(observation(5, FriendId::MAGGIE, 3.0f), 2500);
  friends.update(2500);
  assert(friends.nearestFriend()->id == FriendId::RAHUL);

  FriendManager thresholdFriends;
  thresholdFriends.begin();
  thresholdFriends.observe(observation(5, FriendId::MAGGIE, 3.0f), 3000);
  thresholdFriends.update(3000);
  assert(thresholdFriends.nearestFriend()->id == FriendId::MAGGIE);
  assert(thresholdFriends.nearestFriend()->distanceM == 3.0f);

  return 0;
}
