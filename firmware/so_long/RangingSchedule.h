#pragma once

#include <Arduino.h>

#include "NodeId.h"

constexpr uint32_t RANGING_SLOT_MS = 100;
constexpr uint8_t RANGING_SLOT_COUNT = 2;
constexpr uint32_t RANGING_RESPONSE_TIMEOUT_MS = 5;
constexpr uint32_t RANGING_INITIAL_LISTEN_BASE_MS = 40;
constexpr uint32_t RANGING_NODE_STAGGER_MS = 120;
constexpr uint32_t RANGING_RETRY_MIN_MS = 220;
constexpr uint32_t RANGING_RETRY_MAX_MS = 980;
constexpr uint32_t RANGING_SUCCESS_BASE_MS = 650;
constexpr uint32_t RANGING_RESPONSE_QUIET_BASE_MS = 180;

inline uint32_t rangingSlotForTime(uint32_t nowMs) {
  return nowMs / RANGING_SLOT_MS;
}

inline uint8_t rangingPhaseForNode(NodeId nodeId) {
  if (nodeId == 0) {
    return 0;
  }
  return static_cast<uint8_t>((nodeId - 1) % RANGING_SLOT_COUNT);
}

inline bool isInitiationSlotForNode(NodeId nodeId, uint32_t nowMs) {
  return (rangingSlotForTime(nowMs) % RANGING_SLOT_COUNT) ==
         rangingPhaseForNode(nodeId);
}

inline uint8_t missedInitiationSkipSlots(NodeId nodeId) {
  return rangingPhaseForNode(nodeId) + 1;
}

inline bool rangingTimeReached(uint32_t nowMs, uint32_t dueMs) {
  return static_cast<int32_t>(nowMs - dueMs) >= 0;
}

inline uint32_t rangingStaggerForNode(NodeId nodeId) {
  return static_cast<uint32_t>(rangingPhaseForNode(nodeId)) *
         RANGING_NODE_STAGGER_MS;
}

inline uint32_t rangingInitialListenMs(NodeId nodeId) {
  return RANGING_INITIAL_LISTEN_BASE_MS + rangingStaggerForNode(nodeId);
}

inline uint32_t rangingDeterministicJitterMs(NodeId nodeId, uint8_t attempt) {
  uint32_t value = static_cast<uint32_t>(nodeId) * 1103515245UL;
  value ^= (static_cast<uint32_t>(attempt) + 1UL) * 2654435761UL;
  value ^= value >> 16;

  const uint32_t range = RANGING_RETRY_MAX_MS - RANGING_RETRY_MIN_MS + 1UL;
  return value % range;
}

inline uint32_t rangingRetryDelayMs(NodeId nodeId, uint8_t failedAttempts) {
  return RANGING_RETRY_MIN_MS +
         rangingDeterministicJitterMs(nodeId, failedAttempts);
}

inline uint32_t rangingSuccessDelayMs(NodeId nodeId) {
  return RANGING_SUCCESS_BASE_MS + rangingStaggerForNode(nodeId);
}

inline uint32_t rangingResponseQuietMs(NodeId nodeId) {
  return RANGING_RESPONSE_QUIET_BASE_MS + rangingStaggerForNode(nodeId);
}
