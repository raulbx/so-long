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
- Avoid abstractions until they represent a real distinction in the system.

---

# Domain Model

The domain model represents the concepts of the So-Long world.

```text
Friend
Node
PresenceObservation
Presence
Event
Emotion
EmotionalState
Color
```

The domain model contains no knowledge of:
* radio hardware
* packet layouts
* DW3000
* Arduino
* FastLED
* LED indexes
* animation frames
---
# Color

Color is a hardware-independent RGB value.
```text
struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
```
Domain objects use Color rather than CRGB.
Conversion to FastLED types happens inside the animation layer.
# Emotion

Emotion identifies the heart’s current emotional mode.
```text
enum class Emotion {
    IDLE,
    PRESENT,
    APPROACHING,
    CURIOUS,
    REUNITED,
    DEPARTING
};
```
The emotional modes represent interpretation over time:

* IDLE — no friend is currently present
* PRESENT — a friend is present and no temporary emotional effect is active
* APPROACHING — a visible friend is meaningfully moving closer
* CURIOUS — a friend has newly appeared
* REUNITED — a friend has reached the close-range threshold
* DEPARTING — a previously visible friend has disappeared

APPROACHING is a temporal interpretation. It is based on short-term distance
history rather than a single raw measurement.

# EmotionalState

EmotionalState is the expressive contract between the emotional and animation layers.

It describes what the heart currently feels and the strength of that feeling. It does not describe individual LED positions or frames.

A representative model is:
```text
struct EmotionalState {
    Emotion emotion;

    Color ownerColor;

    bool friendPresent;
    Color friendColor;

    uint8_t intensity;
    uint8_t effectSize;
    uint16_t motionIntervalMs;
};
```
The exact fields may evolve as implementation needs become clearer, but they must remain hardware-independent.

EmotionalState may contain:

* the current emotion
* owner identity color
* friend identity color
* friend presence
* normalized expressive intensity
* high-level effect magnitude
* high-level motion timing

EmotionalState must not contain:

* CRGB
* LED indexes
* FastLED objects
* packet data
* DW3000 data
* raw hardware timestamps
* rendering buffers

A separate VisualState should not be introduced unless a future requirement creates a meaningful distinction between emotional intent and rendering configuration.

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

The protocol knows nothing about:
* emotions
* animations
* LED hardware
* FastLED

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
EmotionalState
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
RangingEngine must not determine emotional state or animation behavior.

---

## FriendManager

Maintains the current understanding of nearby friends.

Responsibilities:

* track observations
* maintain visibility timeouts
* retain the latest valid distance
* maintain the nearest or currently selected friend
* expose current friend context

FriendManager contains no animation logic.
FriendManager should expose objective information such as:

* whether a friend is visible
* friend identity
* latest distance
* observation age
---

## EmotionalStateEngine

Transforms objective friend context into expressive intent.

Responsibilities:

* interpret current and previous friend context
* detect friend arrival
* detect friend disappearance
* detect friend visibility timeout transitions
* detect meaningful inward movement
* detect close-range reunion
* maintain emotional state over time
* determine state transitions
* produce the current EmotionalState

This engine owns temporal interpretation.

Representative internal state may include:
```text
Emotion currentEmotion_;
bool previouslyPresent_;
bool reunionArmed_;
float approachBaselineDistanceM_;
uint32_t stateEnteredAtMs_;
uint32_t approachBaselineAtMs_;
```
The implementation may use additional history or smoothing where needed.

Approach detection must be resistant to noisy UWB readings. It should use a
named observation window, an entry threshold, and a smaller exit threshold or
equivalent hysteresis. Trend history must reset when the selected friend
changes, disappears, or reappears.

REUNITED takes precedence over APPROACHING when the close-range threshold is
crossed.

EmotionalStateEngine must not:

* access DW3000 APIs
* inspect radio packets
* write LED frames
* use FastLED types
* select individual LED positions
* contain comet-rendering logic

This is the boundary between objective observations and subjective experience.

---

## AnimationEngine

Transforms EmotionalState into LED frames.

Responsibilities:

* render the owner-color base layer
* render friend-color overlays
* select the animation associated with each emotion
* calculate LED positions
* calculate frame-level timing
* render comets, fades, pulses, and celebrations
* manage transitions between rendered frames
* convert domain Color values to FastLED CRGB
* send completed frames to FastLED

AnimationEngine may interpret high-level expressive values such as:

* emotion
* intensity
* effect size
* motion interval

AnimationEngine must not:

* inspect radio packets
* inspect PresenceObservation
* inspect FriendId
* calculate friend visibility
* decide whether a reunion occurred
* maintain friend visibility timeouts

---

## FastLED

Drives the LEDs.

No domain, ranging, emotional, or application logic belongs here.

# State Transitions
Emotional transitions are based on observations over time rather than a single measurement.

A representative state flow is:
```text
friend appears
┌────────────────────────────────┐
│                                ▼
IDLE                           CURIOUS
▲                                │
│                                │ observation persists
│ friend absent                  ▼
DEPARTING ◄──── friend lost ───── PRESENT
                                 │
                                 │ meaningful inward movement
                                 ▼
                           APPROACHING
                                 │
                                 │ stable or reversing distance
                                 ▼
                              PRESENT
                                 │
                                 │ close threshold crossed
                                 ▼
                              REUNITED
                                 │
                                 │ celebration expires
                                 ▼
                              PRESENT
```
The exact transition graph may evolve through hardware testing.

Transitions should be:

* deterministic
* testable
* resistant to noisy distance measurements
* controlled by named thresholds and durations
* independent of frame rendering
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

Everything at and below EmotionalStateEngine is expressive.

```text
Emotional interpretation
↓
EmotionalState
↓
Animation
↓
Light
↓
Human experience
```

This boundary is intentional.
The animation engine does not decide what an observation means.
The emotional engine does not decide where an LED should be illuminated.
The heart does not display measurements.

**The heart experiences the world.**
