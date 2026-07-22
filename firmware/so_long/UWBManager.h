#pragma once

#include <Arduino.h>

enum class UWBReceiveStatus : uint8_t {
  FrameReceived,
  TimeoutOrError,
};

class UWBManager {
 public:
  bool begin();
  void transmitAndExpectResponse(uint8_t* data, uint16_t length);
  UWBReceiveStatus waitForReceive();
  void clearGoodReceive();
  void clearReceiveTimeoutOrError();
  uint16_t receivedFrameLength() const;
  void readReceivedFrame(uint8_t* buffer, uint16_t length) const;
  uint32_t txTimestampLo32() const;
  uint32_t rxTimestampLo32() const;
  float clockOffsetRatio() const;
};
