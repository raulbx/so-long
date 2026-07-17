# So-Long Architecture
So-Long is organized into three complementary layers:
- Domain Model
- Communication Protocol
- Firmware
Each layer has a single responsibility.

Domain Model
    │
    ├── Friend
    ├── Node
    ├── Presence
    ├── Event
    └── HeartState
          ▲

Protocol
    │
    ├── Presence Packet
    └── Event Packet
          ▲

Firmware

## Design Principles

- No phones during use
- No GPS
- No internet
- Peer-to-peer first
- Light communicates feeling, not data
- Reliable before fancy
- Magical when friends gather

## Firmware Layers
The firmware transforms radio communication into visual expression.

```text
UWBManager
    ↓
Protocol
    ↓
FriendManager
    ↓
HeartState
    ↓
AnimationEngine
    ↓
FastLED

Then briefly describe each layer:

```markdown
### UWBManager

Interfaces with the DW3000 hardware.

Responsible for:

- Sending packets
- Receiving packets
- Measuring distance

### Protocol

Encodes and decodes protocol packets.

Responsible for:

- Packet parsing
- Versioning
- Packet validation
- Translating bytes into domain concepts

### FriendManager

Maintains the current model of nearby friends.

Responsible for:

- Observations
- Presence
- Visibility timeout
- Nearest friend

### HeartState

Determines the heart's emotional state based on current presence and events.

### AnimationEngine

Maps HeartState to LED animations.

### FastLED

Drives the LEDs.
