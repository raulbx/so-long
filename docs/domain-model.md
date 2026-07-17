# So-Long Domain Model

This document defines the core concepts used throughout So-Long.

It describes the meaning of the system independently of hardware, radio
technology, firmware libraries, and implementation details.

## Heart

A So-Long wearable carried by one person.

A heart represents the complete user-facing experience.

It combines:
- A physical node
- An owner identity
- Nearby-friend awareness
- A visual expression of connection

A heart is the user-facing object. A node is the technical device inside it.

## Friend

A person recognized by the So-Long system.

Each friend has a stable `FriendId`.

A friend may be associated with different physical nodes over the lifetime of
the project. Replacing a damaged node does not change the person's identity.

## Node

A physical So-Long device.

Each node has a `NodeId` that distinguishes it from other physical devices.

A node belongs to one friend.

`NodeId` and `FriendId` are intentionally separate:

- `FriendId` identifies the person.
- `NodeId` identifies the physical device.

## Identity

The association between a node and the friend who owns it.

For example:

- Friend: Rahul
- Node: 1

Identity is local configuration until identities are transmitted through the
So-Long protocol.

## Observation
An observation is a single report received from another node. An observation represents raw, temporary input from the communication and ranging layers.

An observation may contain:

- The observed friend

- The observed node

- The measured distance

- The time the observation occurred

An observation does not by itself mean that a friend is present.

## Presence

Presence is the system's current belief that a friend is nearby.
It is derived from recent observations rather than a single measurement.

A friend remains present for a short period after the latest observation so
that occasional missed radio messages do not cause flickering.

Presence expires when observations have not been received within the configured
visibility timeout.

## Event
An event represents a temporary shared experience between nearby hearts.
Examples include:
- Synchronization
- Celebration
- Beacon

Events are infrequent and separate from continuous presence messages.

Receiving an event does not directly control LEDs. Higher-level application
logic decides whether and how the event affects the heart.

## HeartState

The emotional interpretation of the current situation.

Current states include:

- `AMBIENT`
- `FRIEND_DETECTED`
- `FRIEND_NEAR`
- `FRIEND_FOUND`

Future states may represent shared experiences such as synchronization or group
celebration.

`HeartState` is independent of radio technology and animation implementation.

Shared experiences such as synchronization or group celebration may influence
`HeartState` or may be represented by a separate experience state. This remains
an open design question.

## Animation

The visual expression of a `HeartState`, friend identity, or shared experience.

Animations:

- Must be non-blocking
- Must not depend directly on UWB
- Must not decide whether a friend is present
- Should express meaning rather than raw sensor values

## Presence Packet

A protocol message announcing that a node is nearby and identifying its owner.
A Presence Packet is the protocol representation of presence information. Presence packets are sent periodically.
They update presence information but do not directly trigger animations.

## Event Packet

A protocol message announcing or requesting a temporary shared experience.
An Event Packet is the protocol representation of an event. Event packets are sent infrequently.

Nodes that do not understand an event should safely ignore it.

## Relationships

A simplified view of the domain:

```text
Friend
  owns
    ↓
Node
  sends and receives
    ├── Presence Packets
    └── Event Packets

Presence Packet
      │
      ▼
Observation
      │
      ▼
Presence ─────┐
              │
Event ────────┤
              ▼
         HeartState
              │
              ▼
         Animation

## Guiding Principle
The purpose of the So-Long domain model is to describe people, relationships,
and shared experiences—not implementation details.
Hardware and software may evolve over time, but the concepts defined in this
document should remain stable.
