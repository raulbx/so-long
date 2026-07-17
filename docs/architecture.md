# So-Long Architecture

So-Long is a peer-to-peer wearable light sculpture. Each node is a glowing heart that reacts to nearby friends using UWB distance sensing and expressive LED animations.

## Design Principles

- No phones during use
- No GPS
- No internet
- Peer-to-peer first
- Light communicates feeling, not data
- Reliable before fancy
- Magical when friends gather

## Firmware Layers

```text
UWBManager
    ↓
FriendManager
    ↓
HeartState
    ↓
AnimationEngine
    ↓
FastLED
