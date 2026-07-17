# So-Long

Distance fades. Connection remains.

So-Long is a Burning Man peer-to-peer wearable light sculpture built around ESP32 UWB ranging and addressable neon rope light. Each wearable ranges against nearby friends and translates proximity into light behavior.

## Hardware

- Makerfabs ESP32 UWB DW3000 WROOM board
- DW3000 UWB ranging using the Makerfabs Dw3000 Arduino library
- WS2811/WS2812B 5V neon rope light
- LED data pin: GPIO25
- LED count: 50
- FastLED library

## Firmware Layout

- `firmware/so_long/so_long.ino` - Arduino entry point and current UWB integration area
- `firmware/so_long/Config.h` - hardware and behavior constants
- `firmware/so_long/Friends.h` - friend IDs and assigned colors
- `firmware/so_long/AnimationEngine.h` - animation engine interface
- `firmware/so_long/AnimationEngine.cpp` - FastLED animation implementation

## Current Refactor Stage

The current priority is structure, not changing ranging behavior. Keep the proven Makerfabs ranging logic in `so_long.ino` while the lighting code moves behind `AnimationEngine`.

## Arduino Setup

Open `firmware/so_long/so_long.ino` in Arduino IDE or build it with an Arduino-compatible CLI setup for the Makerfabs ESP32 UWB DW3000 WROOM board.

Install the required libraries:

- FastLED
- Makerfabs Dw3000 Arduino library

