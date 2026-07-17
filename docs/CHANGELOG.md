## v0.4 - Communication Model (Define the protocol and domain model)
### Added

- `docs/protocol.md`
  - Defines the So-Long communication protocol.
  - Separates Presence and Event messages.
  - Establishes protocol design principles and future extensibility.

- `docs/domain-model.md`
  - Defines the core concepts used throughout the project.
  - Separates Heart, Friend, Node, Identity, Presence, Observation, Event, and HeartState.
  - Documents relationships between concepts independently of implementation.

### Architecture

- Established a shared vocabulary for the project.
- Clearly separated domain concepts from protocol representations.
- Positioned future protocol extensions (e.g. synchronization, celebration) without affecting the current implementation.

## v0.3 – Identity

- Added explicit owner identity for each heart.
- Separated human identity (`FriendId`) from physical device identity (`NodeId`).
- Preserved Jennifer as the current prototype responder.
- Kept identity out of the UWB protocol pending protocol design. 

## v0.2 – FriendManager

- Introduced FriendManager as the central source of friend state.
- Separated HeartState from AnimationEngine.
- Added friend visibility timeout.
- Kept prototype behavior unchanged.
