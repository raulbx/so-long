#include <assert.h>
#include <stdint.h>

#include "FriendId.h"
#include "Identity.h"
#include "RangingSchedule.h"

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
