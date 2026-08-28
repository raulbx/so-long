#include <assert.h>
#include <stdint.h>

#include "FriendId.h"
#include "Identity.h"
#include "RangingSchedule.h"

namespace {

struct NodeSimulation {
  NodeId nodeId;
  uint32_t bootAtMs;
  bool booted = false;
  bool waiting = false;
  uint32_t waitingUntilMs = 0;
  uint32_t nextAttemptLocalMs = 0;
  uint8_t failedAttempts = 0;
  uint16_t successes = 0;
};

bool timeReached(uint32_t nowMs, uint32_t dueMs) {
  return static_cast<int32_t>(nowMs - dueMs) >= 0;
}

uint32_t localTimeFor(const NodeSimulation& node, uint32_t realNowMs) {
  return realNowMs - node.bootAtMs;
}

bool tickScheduler(NodeSimulation& node1, NodeSimulation& node2,
                   uint32_t realNowMs) {
  NodeSimulation* nodes[] = {&node1, &node2};
  bool wantsToInitiate[] = {false, false};

  for (uint8_t i = 0; i < 2; ++i) {
    NodeSimulation& node = *nodes[i];
    if (realNowMs < node.bootAtMs) {
      continue;
    }
    if (!node.booted) {
      node.booted = true;
      node.nextAttemptLocalMs = rangingInitialListenMs(node.nodeId);
    }
    if (node.waiting && timeReached(realNowMs, node.waitingUntilMs)) {
      node.waiting = false;
      node.nextAttemptLocalMs =
          localTimeFor(node, realNowMs) +
          rangingRetryDelayMs(node.nodeId, node.failedAttempts++);
    }
    if (!node.waiting &&
        timeReached(localTimeFor(node, realNowMs), node.nextAttemptLocalMs)) {
      wantsToInitiate[i] = true;
    }
  }

  if (wantsToInitiate[0] && wantsToInitiate[1]) {
    node1.waiting = true;
    node2.waiting = true;
    node1.waitingUntilMs = realNowMs + RANGING_RESPONSE_TIMEOUT_MS;
    node2.waitingUntilMs = realNowMs + RANGING_RESPONSE_TIMEOUT_MS;
    return false;
  }

  if (wantsToInitiate[0] || wantsToInitiate[1]) {
    NodeSimulation& initiator = wantsToInitiate[0] ? node1 : node2;
    NodeSimulation& responder = wantsToInitiate[0] ? node2 : node1;
    if (responder.booted && !responder.waiting) {
      initiator.successes++;
      initiator.failedAttempts = 0;
      initiator.nextAttemptLocalMs =
          localTimeFor(initiator, realNowMs) +
          rangingSuccessDelayMs(initiator.nodeId);
      responder.failedAttempts = 0;
      responder.nextAttemptLocalMs =
          localTimeFor(responder, realNowMs) +
          rangingResponseQuietMs(responder.nodeId);
      return true;
    }

    initiator.waiting = true;
    initiator.waitingUntilMs = realNowMs + RANGING_RESPONSE_TIMEOUT_MS;
  }

  return false;
}

bool bothNodesRangeWithin(uint32_t node1BootAtMs, uint32_t node2BootAtMs,
                          uint32_t durationAfterBothBootMs) {
  NodeSimulation node1{1, node1BootAtMs};
  NodeSimulation node2{2, node2BootAtMs};
  const uint32_t startMs =
      node1BootAtMs > node2BootAtMs ? node1BootAtMs : node2BootAtMs;
  const uint32_t endMs = startMs + durationAfterBothBootMs;

  for (uint32_t nowMs = 0; nowMs <= endMs; ++nowMs) {
    tickScheduler(node1, node2, nowMs);
    if (node1.successes > 0 && node2.successes > 0) {
      return true;
    }
  }

  return false;
}

bool legacySlotsConflictForeverWithOffset(uint32_t node2BootOffsetMs) {
  for (uint32_t realNowMs = node2BootOffsetMs + RANGING_SLOT_MS;
       realNowMs < node2BootOffsetMs + 5000; realNowMs += RANGING_SLOT_MS * 2) {
    const uint32_t node1LocalMs = realNowMs;
    const uint32_t node2LocalMs = realNowMs - node2BootOffsetMs;
    if (!isInitiationSlotForNode(1, node1LocalMs) ||
        !isInitiationSlotForNode(2, node2LocalMs)) {
      return false;
    }
  }
  return true;
}

}  // namespace

int main() {
  assert(rangingPhaseForNode(1) == 0);
  assert(rangingPhaseForNode(2) == 1);
  assert(rangingPhaseForNode(3) == 0);

  assert(isInitiationSlotForNode(1, 0));
  assert(!isInitiationSlotForNode(2, 0));
  assert(!isInitiationSlotForNode(1, RANGING_SLOT_MS));
  assert(isInitiationSlotForNode(2, RANGING_SLOT_MS));
  assert(isInitiationSlotForNode(1, RANGING_SLOT_MS * 2));
  assert(!isInitiationSlotForNode(2, RANGING_SLOT_MS * 2));

  assert(missedInitiationSkipSlots(1) == 1);
  assert(missedInitiationSkipSlots(2) == 2);

  assert(legacySlotsConflictForeverWithOffset(RANGING_SLOT_MS));

  assert(rangingInitialListenMs(1) != rangingInitialListenMs(2));
  assert(rangingRetryDelayMs(1, 0) != rangingRetryDelayMs(2, 0));
  assert(rangingRetryDelayMs(1, 0) != rangingRetryDelayMs(1, 1));
  assert(rangingRetryDelayMs(2, 0) != rangingRetryDelayMs(2, 1));
  assert(rangingRetryDelayMs(1, 0) >= RANGING_RETRY_MIN_MS);
  assert(rangingRetryDelayMs(1, 0) <= RANGING_RETRY_MAX_MS);
  assert(rangingRetryDelayMs(2, 5) >= RANGING_RETRY_MIN_MS);
  assert(rangingRetryDelayMs(2, 5) <= RANGING_RETRY_MAX_MS);

  const uint32_t offsets[] = {0, 50, 100, 250, 1000, 10000};
  for (uint32_t offset : offsets) {
    assert(bothNodesRangeWithin(0, offset, 5000));
    assert(bothNodesRangeWithin(offset, 0, 5000));
  }

#if SO_LONG_BOARD_ID == 1
  assert(MY_NODE_ID == 1);
  assert(MY_FRIEND == FriendId::RAHUL);
#elif SO_LONG_BOARD_ID == 2
  assert(MY_NODE_ID == 2);
  assert(MY_FRIEND == FriendId::JENNIFER);
#else
#error Unsupported SO_LONG_BOARD_ID in ranging schedule test.
#endif

  return 0;
}
