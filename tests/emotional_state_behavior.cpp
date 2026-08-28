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

void enterPresent(EmotionalStateEngine& engine, FriendId friendId,
                  float distanceM, uint32_t startMs) {
  FriendObservation observation = friendObservation(friendId, distanceM);
  engine.update(&observation, SoLongColors::Red, startMs);
  engine.update(&observation, SoLongColors::Red,
                startMs + SoLongConfig::CURIOUS_DURATION_MS + 1);
  assert(engine.currentState().emotion == Emotion::PRESENT);
}

void updateDistance(EmotionalStateEngine& engine, FriendId friendId,
                    float distanceM, uint32_t nowMs) {
  FriendObservation observation = friendObservation(friendId, distanceM);
  engine.update(&observation, SoLongColors::Red, nowMs);
}

int main() {
  assert(SoLongConfig::REUNITED_ENTER_DISTANCE_M == 2.0f);
  assert(SoLongConfig::REUNITED_EXIT_DISTANCE_M == 2.5f);

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
      friendObservation(FriendId::JENNIFER, 1.9f);
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

  EmotionalStateEngine stableEngine;
  stableEngine.begin(SoLongColors::Blue);
  enterPresent(stableEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(stableEngine, FriendId::JENNIFER, 4.0f,
                 SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::APPROACHING_WINDOW_MS + 2);
  assert(stableEngine.currentState().emotion == Emotion::PRESENT);

  EmotionalStateEngine noisyEngine;
  noisyEngine.begin(SoLongColors::Blue);
  enterPresent(noisyEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(noisyEngine, FriendId::JENNIFER, 3.85f,
                 SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::APPROACHING_WINDOW_MS + 2);
  assert(noisyEngine.currentState().emotion == Emotion::PRESENT);

  EmotionalStateEngine approachingEngine;
  approachingEngine.begin(SoLongColors::Blue);
  enterPresent(approachingEngine, FriendId::JENNIFER, 4.0f, 0);
  const uint32_t firstTrendWindow =
      SoLongConfig::CURIOUS_DURATION_MS + SoLongConfig::APPROACHING_WINDOW_MS +
      2;
  updateDistance(approachingEngine, FriendId::JENNIFER, 3.4f,
                 firstTrendWindow);
  assert(approachingEngine.currentState().emotion == Emotion::APPROACHING);
  assert(approachingEngine.currentState().effectSize >
         normalPresentStateForDistance(3.4f).effectSize);
  assert(approachingEngine.currentState().intensity >
         normalPresentStateForDistance(3.4f).intensity);
  assert(approachingEngine.currentState().motionIntervalMs <
         normalPresentStateForDistance(3.4f).motionIntervalMs);

  updateDistance(approachingEngine, FriendId::JENNIFER, 2.9f,
                 firstTrendWindow + SoLongConfig::APPROACHING_WINDOW_MS + 1);
  assert(approachingEngine.currentState().emotion == Emotion::APPROACHING);

  updateDistance(approachingEngine, FriendId::JENNIFER, 2.85f,
                 firstTrendWindow + 2 * SoLongConfig::APPROACHING_WINDOW_MS +
                     2);
  assert(approachingEngine.currentState().emotion == Emotion::PRESENT);

  EmotionalStateEngine reversingEngine;
  reversingEngine.begin(SoLongColors::Blue);
  enterPresent(reversingEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(reversingEngine, FriendId::JENNIFER, 3.4f,
                 firstTrendWindow);
  assert(reversingEngine.currentState().emotion == Emotion::APPROACHING);
  updateDistance(reversingEngine, FriendId::JENNIFER, 3.7f,
                 firstTrendWindow + SoLongConfig::APPROACHING_WINDOW_MS + 1);
  assert(reversingEngine.currentState().emotion == Emotion::PRESENT);

  EmotionalStateEngine thresholdEngine;
  thresholdEngine.begin(SoLongColors::Blue);
  enterPresent(thresholdEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(thresholdEngine, FriendId::JENNIFER, 1.99f,
                 SoLongConfig::CURIOUS_DURATION_MS + 2);
  assert(thresholdEngine.currentState().emotion == Emotion::REUNITED);
  thresholdEngine.update(&jenniferClose, SoLongColors::Red,
                         SoLongConfig::CURIOUS_DURATION_MS +
                             SoLongConfig::REUNITED_DURATION_MS + 3);
  assert(thresholdEngine.currentState().emotion == Emotion::PRESENT);
  updateDistance(thresholdEngine, FriendId::JENNIFER, 1.8f,
                 SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::REUNITED_DURATION_MS + 4);
  assert(thresholdEngine.currentState().emotion == Emotion::PRESENT);
  updateDistance(thresholdEngine, FriendId::JENNIFER, 2.6f,
                 SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::REUNITED_DURATION_MS + 5);
  assert(thresholdEngine.currentState().emotion == Emotion::PRESENT);
  updateDistance(thresholdEngine, FriendId::JENNIFER, 1.9f,
                 SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::REUNITED_DURATION_MS + 6);
  assert(thresholdEngine.currentState().emotion == Emotion::REUNITED);

  EmotionalStateEngine lossResetEngine;
  lossResetEngine.begin(SoLongColors::Blue);
  enterPresent(lossResetEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(lossResetEngine, FriendId::JENNIFER, 3.4f,
                 firstTrendWindow);
  assert(lossResetEngine.currentState().emotion == Emotion::APPROACHING);
  lossResetEngine.update(nullptr, SoLongColors::Black, firstTrendWindow + 1);
  assert(lossResetEngine.currentState().emotion == Emotion::DEPARTING);
  lossResetEngine.update(
      nullptr, SoLongColors::Black,
      firstTrendWindow + SoLongConfig::DEPARTING_DURATION_MS + 2);
  assert(lossResetEngine.currentState().emotion == Emotion::IDLE);
  enterPresent(lossResetEngine, FriendId::JENNIFER, 3.4f,
               firstTrendWindow + SoLongConfig::DEPARTING_DURATION_MS + 3);
  updateDistance(lossResetEngine, FriendId::JENNIFER, 3.4f,
                 firstTrendWindow + SoLongConfig::DEPARTING_DURATION_MS +
                     SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::APPROACHING_WINDOW_MS + 5);
  assert(lossResetEngine.currentState().emotion == Emotion::PRESENT);

  EmotionalStateEngine changedFriendEngine;
  changedFriendEngine.begin(SoLongColors::Blue);
  enterPresent(changedFriendEngine, FriendId::JENNIFER, 4.0f, 0);
  updateDistance(changedFriendEngine, FriendId::JENNIFER, 3.4f,
                 firstTrendWindow);
  assert(changedFriendEngine.currentState().emotion == Emotion::APPROACHING);
  updateDistance(changedFriendEngine, FriendId::MIKE, 3.2f,
                 firstTrendWindow + 1);
  assert(changedFriendEngine.currentState().emotion == Emotion::CURIOUS);
  updateDistance(changedFriendEngine, FriendId::MIKE, 3.2f,
                 firstTrendWindow + SoLongConfig::CURIOUS_DURATION_MS + 2);
  assert(changedFriendEngine.currentState().emotion == Emotion::PRESENT);
  updateDistance(changedFriendEngine, FriendId::MIKE, 3.2f,
                 firstTrendWindow + SoLongConfig::CURIOUS_DURATION_MS +
                     SoLongConfig::APPROACHING_WINDOW_MS + 3);
  assert(changedFriendEngine.currentState().emotion == Emotion::PRESENT);

  return 0;
}
