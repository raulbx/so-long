# So-Long Architecture

## Overview

So-Long transforms radio observations into emotional expression.

The system is organized into three complementary layers:

- Domain Model
- Communication Protocol
- Firmware

Each layer has a single responsibility and communicates only through well-defined interfaces.

```text
        Firmware
            │
            ▼
 Communication Protocol
            │
            ▼
      Domain Model
```

---

# Design Principles

- Every class has one responsibility.
- Compile after every meaningful change.
- One architectural change per commit.
- Behavior-preserving refactors whenever possible.
- Prefer composition over inheritance.
- Hardware concerns remain isolated from domain logic.
- Transport-layer types never leak into the domain.
- Domain objects must not depend on Arduino or DW3000 APIs.
- Favor immutable value objects where practical.
- Architecture should optimize for readability over cleverness.

---

# Domain Model

The domain model represents the concepts of the So-Long world.

```text
Friend
Node
PresenceObservation
Presence
Event
HeartState
EmotionalState
```

The domain model contains no knowledge of:

- radio hardware
- packet layouts
- DW3000
- Arduino
- FastLED

---

# Communication Protocol

The protocol translates domain objects into bytes.

```text
Presence Packet
Event Packet
Protocol
```

Responsibilities:

- serialization
- deserialization
- versioning
- packet validation

The protocol knows nothing about animations or hardware.

---

# Firmware

The firmware transforms radio observations into visual expression.

```text
                  Hardware
                     │
                     ▼
                UWBManager
                     │
                     ▼
              RangingEngine
                     │
                     ▼
          PresenceObservation
                     │
                     ▼
              FriendManager
                     │
                     ▼
         EmotionalStateEngine
                     │
                     ▼
             AnimationEngine
                     │
                     ▼
                 FastLED
```

---

## UWBManager

Interfaces directly with the DW3000.

Responsibilities:

- initialize hardware
- configure radio
- transmit raw frames
- receive raw frames
- provide hardware timestamps

UWBManager must not know about:

- FriendId
- PresenceObservation
- HeartState
- animations
- ranging protocol

---

## RangingEngine

Implements the SS-TWR ranging protocol.

Responsibilities:

- initiator / responder state machine
- packet construction
- packet validation
- timestamp extraction
- distance calculation
- identity exchange
- generation of PresenceObservation

RangingEngine owns all protocol timing and radio interaction above the hardware layer.

---

## FriendManager

Maintains the current understanding of nearby friends.

Responsibilities:

- track observations
- maintain visibility timeouts
- maintain nearest friend
- expose current friend context

FriendManager contains no animation logic.

---

## EmotionalStateEngine

Transforms observations into emotional state.

Responsibilities:

- interpret friend context
- detect approaching / departing
- detect reunions
- detect separation
- determine the current EmotionalState

This is the boundary between objective observations and subjective experience.

---

## AnimationEngine

Transforms emotional state into light.

Responsibilities:

- animation selection
- animation timing
- color palettes
- brightness
- transitions

AnimationEngine should never inspect radio packets or ranging data directly.

---

## FastLED

Drives the LEDs.

No application logic belongs here.

---

# Architectural Boundary

Everything above `EmotionalStateEngine` is objective.

```text
Radio
↓
Packets
↓
Distance
↓
PresenceObservation
↓
FriendManager
```

Everything below it is expressive.

```text
EmotionalState
↓
Animation
↓
Light
↓
Human experience
```

This boundary is intentional.

The heart does not display measurements.

**The heart experiences the world.**
