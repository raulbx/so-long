# So-Long Protocol
The So-Long protocol defines how hearts communicate with one another.
The protocol is designed to be:

- Simple
- Reliable
- Extensible
- Human-centric

The goal is not to build a networking protocol.
The goal is to allow hearts to express presence and shared experiences.

## Protocol Philosophy
The So-Long protocol is not designed to move data.
It is designed to communicate presence and create shared experiences.
Every packet should represent a meaningful interaction between people rather than a low-level networking operation.

## Design Principles

- Presence is the primary message.
- Events are rare.
- Unknown fields must be ignored.
- New protocol versions should remain backward compatible whenever possible.
- Every packet should have one clear purpose.
- The protocol should minimize radio airtime.

## Core Concepts

### Heart 
The user-facing wearable containing a node and expressing the system’s emotional state.

### Presence
A heart announcing that it is nearby.

### Event
A temporary action that requests a shared experience.

### Node
A physical wearable device.

### Friend
The human associated with a node.

## Packet Types

### Presence Packet
A Presence packet identifies the sender.
It is transmitted periodically.
Receiving a Presence packet updates the sender's presence information but should not directly trigger animations.

### Event Packet
Event packets are transmitted infrequently.
They request a shared experience.
Examples include:
- Synchronization
- Celebration
- Beacon

## Responsibilities

The protocol is responsible for:
- Identifying nearby hearts.
- Delivering events.
- Remaining lightweight and reliable.

The protocol is NOT responsible for:

- Animation decisions.
- Friend management.
- Emotional state.
- User interface.

## Future Ideas

### Synchronization
Nearby hearts align their animations.

### Celebration
Multiple hearts create a shared animation.

### Find Me
A temporary beacon that helps nearby friends locate one another.

### Battery Awareness
Nodes may optionally advertise battery level.

### Firmware Compatibility
Nodes may optionally advertise protocol or firmware versions.

## Non-goals

The So-Long protocol is not intended to provide:

- Internet connectivity
- Device discovery outside nearby hearts
- High-bandwidth communication
- File transfer
- General-purpose networking

If a feature does not strengthen human connection, it probably does not belong in the protocol.
