#include <assert.h>

#include "Config.h"
#include "EmotionalStateEngine.h"
#include "FriendManager.h"
#include "Identity.h"
#include "OwnerIdentity.h"

bool sameColor(Color left, Color right) {
  return left.red == right.red && left.green == right.green &&
         left.blue == right.blue;
}

FriendObservation friendObservation(FriendId id, float distanceM) {
  return {
      id,
      distanceM,
      0,
      true,
  };
}

PresenceObservation presenceObservation(NodeId nodeId, FriendId friendId,
                                        float distanceMeters) {
  return {
      nodeId,
      friendId,
      distanceMeters,
  };
}

EmotionalState normalPresentStateForDistance(float distanceM) {
  EmotionalStateEngine engine;
  engine.begin(SoLongColors::Blue);

  FriendObservation observation =
      friendObservation(FriendId::JENNIFER, distanceM);
  engine.update(&observation, SoLongColors::Red, 0);
  engine.update(&observation, SoLongColors::Red,
                SoLongConfig::CURIOUS_DURATION_MS + 1);

  return engine.currentState();
}

int main() {
  EmotionalStateEngine engine;
  engine.begin(SoLongColors::Blue);

  engine.update(nullptr, SoLongColors::Black, 0);
  EmotionalState state = engine.currentState();
  assert(state.emotion == Emotion::IDLE);
  assert(!state.friendPresent);
  assert(sameColor(state.ownerColor, SoLongColors::Blue));

  FriendObservation jenniferFar =
      friendObservation(FriendId::JENNIFER, 4.0f);
  engine.update(&jenniferFar, SoLongColors::Red, 100);
  state = engine.currentState();
  assert(state.emotion == Emotion::CURIOUS);
  assert(state.friendPresent);
  assert(sameColor(state.friendColor, SoLongColors::Red));

  engine.update(&jenniferFar, SoLongColors::Red,
                100 + SoLongConfig::CURIOUS_DURATION_MS + 1);
  state = engine.currentState();
  assert(state.emotion == Emotion::PRESENT);
  assert(state.friendPresent);
  assert(state.effectSize == SoLongConfig::COMET_FAR_LENGTH);
  assert(state.motionIntervalMs == SoLongConfig::COMET_SLOW_MS);

  FriendObservation jenniferClose =
      friendObservation(FriendId::JENNIFER, 0.75f);
  engine.update(&jenniferClose, SoLongColors::Red,
                100 + SoLongConfig::CURIOUS_DURATION_MS + 2);
  state = engine.currentState();
  assert(state.emotion == Emotion::REUNITED);
  assert(state.friendPresent);

  const uint32_t afterReunion =
      100 + SoLongConfig::CURIOUS_DURATION_MS +
      SoLongConfig::REUNITED_DURATION_MS + 3;
  engine.update(&jenniferClose, SoLongColors::Red, afterReunion);
  state = engine.currentState();
  assert(state.emotion == Emotion::PRESENT);

  engine.update(&jenniferClose, SoLongColors::Red, afterReunion + 10);
  assert(engine.currentState().emotion == Emotion::PRESENT);

  FriendObservation jenniferOutsideHysteresis = friendObservation(
      FriendId::JENNIFER, SoLongConfig::REUNITED_EXIT_DISTANCE_M + 0.1f);
  engine.update(&jenniferOutsideHysteresis, SoLongColors::Red,
                afterReunion + 20);
  assert(engine.currentState().emotion == Emotion::PRESENT);

  engine.update(&jenniferClose, SoLongColors::Red, afterReunion + 30);
  assert(engine.currentState().emotion == Emotion::REUNITED);

  FriendManager friends;
  friends.begin();
  EmotionalStateEngine departureEngine;
  departureEngine.begin(SoLongColors::Blue);

  friends.observe(presenceObservation(2, FriendId::JENNIFER, 4.0f), 0);
  friends.update(0);
  departureEngine.update(friends.nearestFriend(), SoLongColors::Red, 0);
  departureEngine.update(friends.nearestFriend(), SoLongColors::Red,
                         SoLongConfig::CURIOUS_DURATION_MS + 1);
  assert(departureEngine.currentState().emotion == Emotion::PRESENT);

  const uint32_t timeoutAt =
      SoLongConfig::FRIEND_VISIBILITY_TIMEOUT_MS +
      SoLongConfig::CURIOUS_DURATION_MS + 2;
  friends.update(timeoutAt);
  departureEngine.update(friends.nearestFriend(), SoLongColors::Red,
                         timeoutAt);
  state = departureEngine.currentState();
  assert(state.emotion == Emotion::DEPARTING);
  assert(!state.friendPresent);
  assert(sameColor(state.friendColor, SoLongColors::Red));

  departureEngine.update(nullptr, SoLongColors::Red,
                         timeoutAt + SoLongConfig::DEPARTING_DURATION_MS + 1);
  state = departureEngine.currentState();
  assert(state.emotion == Emotion::IDLE);
  assert(!state.friendPresent);

  const FriendInfo* owner = localOwnerInfo();
  assert(owner != nullptr);
  EmotionalStateEngine ownerEngine;
  ownerEngine.begin(owner->color);
  ownerEngine.update(nullptr, SoLongColors::Black, 0);

#if SO_LONG_BOARD_ID == 1
  assert(MY_FRIEND == FriendId::RAHUL);
  assert(sameColor(ownerEngine.currentState().ownerColor, SoLongColors::Blue));
#elif SO_LONG_BOARD_ID == 2
  assert(MY_FRIEND == FriendId::JENNIFER);
  assert(sameColor(ownerEngine.currentState().ownerColor, SoLongColors::Red));
#else
#error Unsupported SO_LONG_BOARD_ID in emotional state behavior test.
#endif

  const FriendInfo* jenniferInfo = friendInfoFor(FriendId::JENNIFER);
  assert(jenniferInfo != nullptr);
  ownerEngine.update(&jenniferFar, jenniferInfo->color, 10);
  assert(sameColor(ownerEngine.currentState().friendColor,
                   jenniferInfo->color));

  const EmotionalState farPresent = normalPresentStateForDistance(4.0f);
  const EmotionalState nearPresent = normalPresentStateForDistance(2.0f);
  const EmotionalState closePresent = normalPresentStateForDistance(0.75f);
  assert(farPresent.emotion == Emotion::PRESENT);
  assert(nearPresent.emotion == Emotion::PRESENT);
  assert(closePresent.emotion == Emotion::PRESENT);
  assert(farPresent.effectSize < nearPresent.effectSize);
  assert(nearPresent.effectSize < closePresent.effectSize);
  assert(farPresent.intensity < nearPresent.intensity);
  assert(nearPresent.intensity < closePresent.intensity);
  assert(farPresent.motionIntervalMs == SoLongConfig::COMET_SLOW_MS);
  assert(nearPresent.motionIntervalMs == SoLongConfig::COMET_MEDIUM_MS);
  assert(closePresent.motionIntervalMs == SoLongConfig::COMET_FAST_MS);

  return 0;
}
