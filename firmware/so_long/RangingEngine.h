#pragma once

#include <Arduino.h>

#include "FriendId.h"
#include "NodeId.h"
#include "PresenceObservation.h"
#include "UWBManager.h"

enum class RangingState : uint8_t {
  Listening,
  Initiating,
  WaitingForResponse,
  SendingResponse,
};

// RangingEngine owns the SS-TWR exchange above the DW3000 transport.
class RangingEngine {
 public:
  RangingEngine(UWBManager& uwb, NodeId localNodeId, FriendId localFriendId);

  bool update();
  float latestDistanceMeters() const;
  PresenceObservation latestObservation() const;

 private:
  bool updateListening(uint32_t nowMs);
  bool updateInitiating(uint32_t nowMs);
  bool updateWaitingForResponse();
  void updateSendingResponse();
  bool readReceivedFrame(uint16_t* frameLen);
  bool handlePollFrame(uint16_t frameLen);
  bool handleResponseFrame(uint16_t frameLen);
  bool serializeLocalPresence(uint8_t* destination);
  void returnToListening();

  UWBManager& uwb_;
  const NodeId localNodeId_;
  const FriendId localFriendId_;
  RangingState state_ = RangingState::Listening;
  bool listeningEnabled_ = false;
  uint8_t frameSeq_ = 0;
  uint32_t lastInitiatedSlot_ = UINT32_MAX;
  uint8_t slotsToSkip_ = 0;
  uint32_t lastRangeAttemptMs_ = 0;
  float latestDistanceMeters_ = -1.0f;
  float filteredDistanceMeters_ = -1.0f;
  PresenceObservation latestObservation_ = {0, FriendId::RAHUL, -1.0f};
};
