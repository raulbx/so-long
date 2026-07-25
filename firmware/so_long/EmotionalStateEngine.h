#pragma once

#include <stdint.h>

#include "DomainColor.h"
#include "EmotionalState.h"
#include "FriendId.h"
#include "FriendManager.h"

class EmotionalStateEngine {
 public:
  void begin(Color ownerColor);
  void update(const FriendObservation* observation, Color friendColor,
              uint32_t nowMs);

  EmotionalState currentState() const;

 private:
  Color ownerColor_ = SoLongColors::DeepSkyBlue;
  Color lastFriendColor_ = SoLongColors::Black;
  FriendId activeFriendId_ = FriendId::RAHUL;
  bool hasActiveFriend_ = false;
  bool reunionArmed_ = true;
  bool approachTrendInitialized_ = false;
  float latestDistanceM_ = -1.0f;
  float approachBaselineDistanceM_ = -1.0f;
  Emotion currentEmotion_ = Emotion::IDLE;
  uint32_t stateEnteredAtMs_ = 0;
  uint32_t approachBaselineAtMs_ = 0;
  EmotionalState state_ = {
      Emotion::IDLE,
      SoLongColors::DeepSkyBlue,
      false,
      SoLongColors::Black,
      0,
      0,
      0,
  };

  void enter(Emotion emotion, uint32_t nowMs);
  bool stateExpired(uint32_t nowMs, uint32_t durationMs) const;
  void updateVisibleFriend(const FriendObservation& observation,
                           Color friendColor, uint32_t nowMs);
  void updateNoVisibleFriend(uint32_t nowMs);
  void resetApproachTrend(float distanceM, uint32_t nowMs);
  void updateApproachTrend(float distanceM, uint32_t nowMs);
  void publishVisibleState(bool friendPresent);
  void publishIdleState();
  void publishDepartingState(uint32_t nowMs);
  uint8_t boostedEffectSize(uint8_t effectSize) const;
  uint8_t boostedIntensity(uint8_t intensity) const;
  uint16_t boostedMotionInterval(uint16_t motionIntervalMs) const;
  uint8_t effectSizeForDistance(float distanceM) const;
  uint8_t intensityForDistance(float distanceM) const;
  uint16_t motionIntervalForDistance(float distanceM) const;
  uint8_t departingIntensity(uint32_t nowMs) const;
};
