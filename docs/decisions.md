# Engineering Principles

* Emotion over technology.
* Reliability over cleverness.
* Simple code over smart code.
* Every layer has a single responsibility.
* Hardware should disappear; connection should remain.
 
# So-Long Architecture Decisions

This document records the major architectural decisions made during the
development of So-Long.

Each decision should explain:

- What was decided
- Why it was chosen
- Alternatives considered
- Consequences

# So-Long Architecture Decisions

This document records the major architectural decisions made during the
development of So-Long.

Each decision should explain:

- What was decided
- Why it was chosen
- Alternatives considered
- Consequences

## ADR-001: Layered Architecture

Status: Accepted
Date: 2026-07-17

### Decision

Separate the firmware into independent layers:

UWBManager
↓
FriendManager
↓
HeartState
↓
AnimationEngine

### Why

Each layer should have a single responsibility.

- UWBManager handles radio communication and ranging.
- FriendManager tracks friends and determines proximity.
- HeartState translates proximity into an emotional state.
- AnimationEngine renders visual effects.

### Alternatives

A single .ino file containing all logic.

### Consequences

The code is easier to test, extend, and reason about.
New sensors or animations can be added without affecting unrelated code.

## ADR-002: HeartState represents emotion instead of distance

Status: Accepted

### Decision

AnimationEngine receives HeartState rather than raw UWB distance.

### Why

Animations should express emotion rather than sensor data.

The animation should not know whether the input came from UWB,
Bluetooth, GPS, or another source.

### Consequences

The sensing layer and artistic layer remain independent.

## ADR-003: Friend visibility timeout

Status: Accepted

### Decision

Friends remain visible for 2 seconds after the last successful ranging
measurement.

### Why

UWB occasionally misses packets.

A short timeout prevents flickering between visible and invisible states.

### Consequences

The experience feels smoother while still reacting quickly when friends leave.

## ADR-004: Fixed-size friend storage

Status: Accepted

### Decision

FriendManager uses fixed-size storage instead of dynamic allocation.

### Why

Embedded systems benefit from deterministic memory usage and avoid heap
fragmentation.

### Consequences

The maximum number of friends is fixed at compile time.

## ADR-005: Non-blocking animations

Status: Accepted

### Decision

Animations must never use delay().

### Why

Blocking animations interfere with UWB ranging and reduce responsiveness.

### Consequences

All animations are driven from millis().

## ADR-006 — Separate Domain Identity Types

### Context

FriendId was originally defined inside Friends.h, which also contained LED color information. This created an unnecessary dependency between the domain model and the rendering layer.

### Decision

Move FriendId into its own lightweight header. Introduce NodeId as a separate domain type. Identity.h now configures the local node rather than defining shared domain types.

### Consequences

* Protocol can depend on domain identity without pulling in FastLED.
* Rendering concerns remain isolated.
* Future protocol and simulation code can reuse the domain model independently of hardware.

# Future Decisions

- How should board identities be assigned?
- Should NodeId and FriendId be separate concepts?
- How should friends be discovered?
- How should UWB packets be structured?
- Should animations be synchronized between boards?
- Should boards communicate over pure UWB or use BLE for configuration?
- How should battery level be represented?
