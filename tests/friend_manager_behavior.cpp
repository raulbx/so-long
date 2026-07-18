#include <assert.h>
#include "FriendManager.h"

int main() {
  FriendManager friends;
  friends.begin();

  friends.update(0);
  assert(!friends.hasVisibleFriend());
  assert(friends.nearestFriend() == nullptr);
  assert(friends.heartState() == HeartState::AMBIENT);

  friends.observe(FriendId::JENNIFER, 4.0f, 100);
  friends.update(100);
  assert(friends.hasVisibleFriend());
  assert(friends.nearestFriend()->id == FriendId::JENNIFER);
  assert(friends.heartState() == HeartState::FRIEND_DETECTED);

  friends.observe(FriendId::MIKE, 2.0f, 150);
  friends.update(150);
  assert(friends.nearestFriend()->id == FriendId::MIKE);
  assert(friends.heartState() == HeartState::FRIEND_NEAR);

  friends.observe(FriendId::MAX, 0.5f, 200);
  friends.update(200);
  assert(friends.nearestFriend()->id == FriendId::MAX);
  assert(friends.heartState() == HeartState::FRIEND_FOUND);

  friends.update(2401);
  assert(!friends.hasVisibleFriend());
  assert(friends.nearestFriend() == nullptr);
  assert(friends.heartState() == HeartState::AMBIENT);

  friends.observe(FriendId::RAHUL, 1.0f, 2500);
  friends.update(2500);
  assert(friends.nearestFriend()->id == FriendId::RAHUL);
  assert(friends.heartState() == HeartState::FRIEND_NEAR);

  friends.observe(FriendId::MAGGIE, 3.0f, 2500);
  friends.update(2500);
  assert(friends.nearestFriend()->id == FriendId::RAHUL);

  FriendManager thresholdFriends;
  thresholdFriends.begin();
  thresholdFriends.observe(FriendId::MAGGIE, 3.0f, 3000);
  thresholdFriends.update(3000);
  assert(thresholdFriends.nearestFriend()->id == FriendId::MAGGIE);
  assert(thresholdFriends.heartState() == HeartState::FRIEND_DETECTED);

  return 0;
}
