# Vision

So-Long is a wearable signal for real-world connection in a noisy, crowded environment. Each person carries a light sculpture that shifts from ambient presence into friend-aware behavior as another wearable comes into UWB range.

The firmware should stay simple enough to repair and evolve in the field:

- UWB ranging remains isolated and proven before deeper changes.
- Animation state is independent from ranging implementation details.
- Friend identity, colors, thresholds, and hardware constants are easy to change.
- The main loop stays responsive and avoids animation delays.

The long-term experience is a light language:

- Ambient mode when no friend is nearby.
- Friend detected when a known wearable appears.
- Friend near as distance closes.
- Friend found when the wearable is very close.

