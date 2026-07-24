#pragma once

#include <Arduino.h>

#include "NodeId.h"

constexpr uint32_t RANGING_SLOT_MS = 100;
constexpr uint8_t RANGING_SLOT_COUNT = 2;

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
