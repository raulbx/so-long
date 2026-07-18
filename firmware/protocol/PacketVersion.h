#pragma once

#include <Arduino.h>

// ProtocolVersion identifies the wire format used by a packet.
enum class ProtocolVersion : uint8_t {
  V1 = 1,
};
