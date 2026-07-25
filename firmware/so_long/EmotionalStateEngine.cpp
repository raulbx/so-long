#include "EmotionalStateEngine.h"

#include "Config.h"

void EmotionalStateEngine::begin(Color ownerColor) {
  ownerColor_ = ownerColor;
  lastFriendColor_ = SoLongColors::Black;
  activeFriendId_ = FriendId::RAHUL;
  hasActiveFriend_ = false;
  reunionArmed_ = true;
  latestDistanceM_ = -1.0f;
  currentEmotion_ = Emotion::IDLE;
  stateEnteredAtMs_ = 0;
  publishIdleState();
}

void EmotionalStateEngine::update(const FriendObservation* observation,
                                  Color friendColor, uint32_t nowMs) {
  if (observation != nullptr && observation->visible) {
    updateVisibleFriend(*observation, friendColor, nowMs);
    return;
  }

  updateNoVisibleFriend(nowMs);
}

EmotionalState EmotionalStateEngine::currentState() const {
  return state_;
}

void EmotionalStateEngine::enter(Emotion emotion, uint32_t nowMs) {
  if (currentEmotion_ == emotion) {
    return;
  }

  currentEmotion_ = emotion;
  stateEnteredAtMs_ = nowMs;
}

bool EmotionalStateEngine::stateExpired(uint32_t nowMs,
                                        uint32_t durationMs) const {
  return nowMs - stateEnteredAtMs_ >= durationMs;
}

void EmotionalStateEngine::updateVisibleFriend(
    const FriendObservation& observation, Color friendColor, uint32_t nowMs) {
  const bool isNewFriend =
      !hasActiveFriend_ || observation.id != activeFriendId_;

  activeFriendId_ = observation.id;
  hasActiveFriend_ = true;
  lastFriendColor_ = friendColor;
  latestDistanceM_ = observation.distanceM;

  if (isNewFriend) {
    reunionArmed_ =
        observation.distanceM >= SoLongConfig::REUNITED_EXIT_DISTANCE_M;
    enter(Emotion::CURIOUS, nowMs);
    publishVisibleState(true);
    return;
  }

  if (observation.distanceM >= SoLongConfig::REUNITED_EXIT_DISTANCE_M) {
    reunionArmed_ = true;
  }

  if (currentEmotion_ == Emotion::DEPARTING) {
    reunionArmed_ =
        observation.distanceM >= SoLongConfig::REUNITED_EXIT_DISTANCE_M;
    enter(Emotion::CURIOUS, nowMs);
    publishVisibleState(true);
    return;
  }

  if (currentEmotion_ == Emotion::CURIOUS &&
      stateExpired(nowMs, SoLongConfig::CURIOUS_DURATION_MS)) {
    enter(Emotion::PRESENT, nowMs);
  }

  if (currentEmotion_ == Emotion::REUNITED &&
      stateExpired(nowMs, SoLongConfig::REUNITED_DURATION_MS)) {
    enter(Emotion::PRESENT, nowMs);
  }

  if (currentEmotion_ == Emotion::PRESENT && reunionArmed_ &&
      observation.distanceM < SoLongConfig::REUNITED_ENTER_DISTANCE_M) {
    reunionArmed_ = false;
    enter(Emotion::REUNITED, nowMs);
  }

  publishVisibleState(true);
}

void EmotionalStateEngine::updateNoVisibleFriend(uint32_t nowMs) {
  if (hasActiveFriend_) {
    hasActiveFriend_ = false;
    latestDistanceM_ = -1.0f;
    reunionArmed_ = true;
    enter(Emotion::DEPARTING, nowMs);
    publishDepartingState(nowMs);
    return;
  }

  if (currentEmotion_ == Emotion::DEPARTING &&
      !stateExpired(nowMs, SoLongConfig::DEPARTING_DURATION_MS)) {
    publishDepartingState(nowMs);
    return;
  }

  enter(Emotion::IDLE, nowMs);
  publishIdleState();
}

void EmotionalStateEngine::publishVisibleState(bool friendPresent) {
  uint8_t intensity = intensityForDistance(latestDistanceM_);
  uint8_t effectSize = effectSizeForDistance(latestDistanceM_);
  uint16_t motionIntervalMs = motionIntervalForDistance(latestDistanceM_);

  if (currentEmotion_ == Emotion::CURIOUS) {
    intensity = SoLongConfig::CURIOUS_INTENSITY;
    effectSize = SoLongConfig::CURIOUS_EFFECT_SIZE;
    motionIntervalMs = SoLongConfig::COMET_MEDIUM_MS;
  } else if (currentEmotion_ == Emotion::REUNITED) {
    intensity = SoLongConfig::REUNITED_INTENSITY;
    effectSize = SoLongConfig::REUNITED_EFFECT_SIZE;
    motionIntervalMs = SoLongConfig::COMET_FAST_MS;
  }

  state_ = {
      currentEmotion_,
      ownerColor_,
      friendPresent,
      lastFriendColor_,
      intensity,
      effectSize,
      motionIntervalMs,
  };
}

void EmotionalStateEngine::publishIdleState() {
  state_ = {
      Emotion::IDLE,
      ownerColor_,
      false,
      SoLongColors::Black,
      0,
      0,
      SoLongConfig::COMET_SLOW_MS,
  };
}

void EmotionalStateEngine::publishDepartingState(uint32_t nowMs) {
  state_ = {
      Emotion::DEPARTING,
      ownerColor_,
      false,
      lastFriendColor_,
      departingIntensity(nowMs),
      SoLongConfig::DEPARTING_EFFECT_SIZE,
      SoLongConfig::COMET_SLOW_MS,
  };
}

uint8_t EmotionalStateEngine::effectSizeForDistance(float distanceM) const {
  if (distanceM < SoLongConfig::REUNITED_ENTER_DISTANCE_M) {
    return SoLongConfig::COMET_CLOSE_LENGTH;
  }

  if (distanceM < 3.0f) {
    return SoLongConfig::COMET_NEAR_LENGTH;
  }

  return SoLongConfig::COMET_FAR_LENGTH;
}

uint8_t EmotionalStateEngine::intensityForDistance(float distanceM) const {
  if (distanceM < SoLongConfig::REUNITED_ENTER_DISTANCE_M) {
    return SoLongConfig::COMET_CLOSE_INTENSITY;
  }

  if (distanceM < 3.0f) {
    return SoLongConfig::COMET_NEAR_INTENSITY;
  }

  return SoLongConfig::COMET_FAR_INTENSITY;
}

uint16_t EmotionalStateEngine::motionIntervalForDistance(
    float distanceM) const {
  if (distanceM < SoLongConfig::REUNITED_ENTER_DISTANCE_M) {
    return SoLongConfig::COMET_FAST_MS;
  }

  if (distanceM < 3.0f) {
    return SoLongConfig::COMET_MEDIUM_MS;
  }

  return SoLongConfig::COMET_SLOW_MS;
}

uint8_t EmotionalStateEngine::departingIntensity(uint32_t nowMs) const {
  const uint32_t elapsed = nowMs - stateEnteredAtMs_;
  if (elapsed >= SoLongConfig::DEPARTING_DURATION_MS) {
    return 0;
  }

  const uint32_t remaining = SoLongConfig::DEPARTING_DURATION_MS - elapsed;
  return static_cast<uint8_t>((255UL * remaining) /
                              SoLongConfig::DEPARTING_DURATION_MS);
}
