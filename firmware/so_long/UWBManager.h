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
  bool transmitDelayed(uint8_t* data, uint16_t length);
  bool enableReceive();
  uint32_t readStatus() const;
  bool hasReceivedFrame(uint32_t status) const;
  bool hasReceiveTimeoutOrError(uint32_t status) const;
  bool hasTransmitComplete(uint32_t status) const;
  UWBReceiveStatus waitForReceive();
  void clearGoodReceive();
  void clearReceiveTimeoutOrError();
  void clearTransmitComplete();
  uint16_t receivedFrameLength() const;
  void readReceivedFrame(uint8_t* buffer, uint16_t length) const;
  uint32_t txTimestampLo32() const;
  uint32_t rxTimestampLo32() const;
  uint64_t rxTimestamp64() const;
  float clockOffsetRatio() const;
  void setDelayedTransmitTime(uint32_t txTime) const;
  uint16_t txAntennaDelay() const;
};
