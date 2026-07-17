# Roadmap

## Phase 1: Structure

- Create an Arduino-compatible project layout.
- Preserve working Makerfabs DW3000 ranging logic in `so_long.ino`.
- Move LED animation into `AnimationEngine`.
- Centralize pin, LED, friend, and threshold configuration.

## Phase 2: Ranging Integration

- Identify the local wearer with `FriendId`.
- Map remote UWB identity to `FriendInfo`.
- Convert measured distance into `HeartState`.
- Keep responder and initiator roles explicit until the prototype behavior is stable.

## Phase 3: Peer Behavior

- Support multiple friends.
- Add per-friend color blending when more than one friend is nearby.
- Add timeout handling for stale range measurements.
- Add serial diagnostics for friend identity, range, and state.

## Phase 4: Wearable Polish

- Tune animations for neon rope diffusion.
- Add power-conscious brightness limits.
- Add field-test presets for night visibility and battery life.
- Document build, flashing, and troubleshooting steps.

