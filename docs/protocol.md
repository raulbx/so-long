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

## Versioning
Every packet begins with a protocol version
Nodes ignore packets using unsupported protocol versions.
New protocol versions should preserve backward compatibility whenever practical.

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

#### Purpose
The Presence Packet periodically announces the identity of a heart to nearby peers. It is intentionally minimal and contains only the information required for nearby hearts to recognize one another.

#### Why It Exists
Presence is the foundation of the So-Long experience. Every other interaction builds on first knowing that another heart is nearby.

#### Wire Format
| Byte | Field | Description |
|------|-------|-------------|
| 0 | Version | Protocol version |
| 1 | NodeId | Physical hardware identifier |
| 2 | FriendId | Human identity |
All fields are unsigned 8-bit values unless otherwise specified.

#### Serialization Rules
- Version is always the first byte.
- Unknown protocol versions are ignored.
- Unknown FriendIds are ignored.
- Unknown NodeIds are ignored.
- Packets are fixed length.

#### Packet Lifecycle
1. Heart creates a Presence Packet.
2. Packet is serialized.
3. Packet is transmitted over UWB.
4. Nearby hearts deserialize the packet.
5. The packet becomes an Observation.
6. FriendManager updates presence.

### Event Packet
Event packets are transmitted infrequently.
They request a shared experience.
Examples include:
- Synchronization
- Celebration
- Beacon

## Responsibilities

The protocol is responsible for:
- Communicating identity between nearby hearts.
- Delivering shared events.
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

### Optional Metadata
- Battery level
- Firmware version

## Non-goals

The So-Long protocol is not intended to provide:

- Internet connectivity
- Device discovery outside nearby hearts
- High-bandwidth communication
- File transfer
- General-purpose networking

If a feature does not strengthen human connection, it probably does not belong in the protocol.
