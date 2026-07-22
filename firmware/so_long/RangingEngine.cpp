#include "RangingEngine.h"

#include "../protocol/Protocol.h"
#include "dw3000.h"

#include <string.h>

#define RNG_DELAY_MS 100
#define ALL_MSG_COMMON_LEN 10
#define ALL_MSG_SN_IDX 2
#define RESP_MSG_POLL_RX_TS_IDX 10
#define RESP_MSG_RESP_TX_TS_IDX 14
#define RESP_MSG_TS_LEN 4
#define POLL_MSG_PRESENCE_IDX 10
#define RESP_MSG_PRESENCE_IDX 18
#define POLL_MSG_FCS_IDX 13
#define RESP_MSG_FCS_IDX 21
#define POLL_RX_TO_RESP_TX_DLY_UUS 1200

static uint8_t tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0};
static uint8_t rx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0};
static uint8_t rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t tx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t rx_buffer[23];
static double tof;
static double distance;

RangingEngine::RangingEngine(UWBManager& uwb, NodeId localNodeId,
                             FriendId localFriendId)
    : uwb_(uwb),
      localNodeId_(localNodeId),
      localFriendId_(localFriendId) {}

bool RangingEngine::update() {
  const uint32_t nowMs = millis();

  switch (state_) {
    case RangingState::Listening:
      return updateListening(nowMs);
    case RangingState::Initiating:
      return updateInitiating(nowMs);
    case RangingState::WaitingForResponse:
      return updateWaitingForResponse();
    case RangingState::SendingResponse:
      updateSendingResponse();
      return false;
  }

  return false;
}

bool RangingEngine::updateListening(uint32_t nowMs) {
  if (!listeningEnabled_) {
    listeningEnabled_ = uwb_.enableReceive();
  }

  const uint32_t status = uwb_.readStatus();
  if (uwb_.hasReceivedFrame(status)) {
    uint16_t frame_len;
    const bool frameRead = readReceivedFrame(&frame_len);
    if (frameRead && handlePollFrame(frame_len)) {
      return false;
    }
    returnToListening();
    return false;
  }

  if (uwb_.hasReceiveTimeoutOrError(status)) {
    uwb_.clearReceiveTimeoutOrError();
    listeningEnabled_ = false;
  }

  if (nowMs - lastRangeAttemptMs_ >= RNG_DELAY_MS) {
    state_ = RangingState::Initiating;
    return updateInitiating(nowMs);
  }

  return false;
}

bool RangingEngine::updateInitiating(uint32_t nowMs) {
  lastRangeAttemptMs_ = nowMs;
  listeningEnabled_ = false;

  if (!serializeLocalPresence(&tx_poll_msg[POLL_MSG_PRESENCE_IDX])) {
    returnToListening();
    return false;
  }

  /* Write frame data to DW IC and prepare transmission. See NOTE 7 below. */
  tx_poll_msg[ALL_MSG_SN_IDX] = frameSeq_;
  uwb_.transmitAndExpectResponse(tx_poll_msg, sizeof(tx_poll_msg));
  state_ = RangingState::WaitingForResponse;

  return false;
}

bool RangingEngine::updateWaitingForResponse() {
  const uint32_t status = uwb_.readStatus();
  if (uwb_.hasReceivedFrame(status)) {
    /* Increment frame sequence number after transmission of the poll message (modulo 256). */
    frameSeq_++;
    uint16_t frame_len;
    const bool frameRead = readReceivedFrame(&frame_len);
    const bool validObservationCalculated =
        frameRead && handleResponseFrame(frame_len);
    returnToListening();
    return validObservationCalculated;
  }

  if (uwb_.hasReceiveTimeoutOrError(status)) {
    /* Increment frame sequence number after transmission of the poll message (modulo 256). */
    frameSeq_++;
    /* Clear RX error/timeout events in the DW IC status register. */
    // dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
    // Serial.print("RX timeout/error. Status = 0x");
    // Serial.println(status_reg, HEX);
    uwb_.clearReceiveTimeoutOrError();
    returnToListening();
  }

  return false;
}

void RangingEngine::updateSendingResponse() {
  const uint32_t status = uwb_.readStatus();
  if (uwb_.hasTransmitComplete(status)) {
    uwb_.clearTransmitComplete();

    /* Increment frame sequence number after transmission of the response message (modulo 256). */
    frameSeq_++;
    returnToListening();
  }
}

bool RangingEngine::readReceivedFrame(uint16_t* frameLen) {
  /* Clear good RX frame event in the DW IC status register. */
  uwb_.clearGoodReceive();

  /* A frame has been received, read it into the local buffer. */
  *frameLen = uwb_.receivedFrameLength();
  if (*frameLen > sizeof(rx_buffer)) {
    return false;
  }

  uwb_.readReceivedFrame(rx_buffer, *frameLen);
  return true;
}

bool RangingEngine::handlePollFrame(uint16_t frameLen) {
  /* Check that the frame is a poll sent by "SS TWR initiator" example.
   * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
  rx_buffer[ALL_MSG_SN_IDX] = 0;
  if (memcmp(rx_buffer, rx_poll_msg, ALL_MSG_COMMON_LEN) != 0) {
    return false;
  }

  if (frameLen < POLL_MSG_PRESENCE_IDX + Protocol::PRESENCE_PACKET_SIZE) {
    return false;
  }

  const PresencePacketResult result =
      Protocol::deserialize(&rx_buffer[POLL_MSG_PRESENCE_IDX],
                            Protocol::PRESENCE_PACKET_SIZE);
  if (!result.valid) {
    return false;
  }

  uint32_t resp_tx_time;
  uint64_t poll_rx_ts;
  uint64_t resp_tx_ts;

  /* Retrieve poll reception timestamp. */
  poll_rx_ts = uwb_.rxTimestamp64();

  /* Compute response message transmission time. See NOTE 7 below. */
  resp_tx_time = (poll_rx_ts + (POLL_RX_TO_RESP_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
  uwb_.setDelayedTransmitTime(resp_tx_time);

  /* Response TX timestamp is the transmission time we programmed plus the antenna delay. */
  resp_tx_ts = (((uint64_t)(resp_tx_time & 0xFFFFFFFEUL)) << 8) + uwb_.txAntennaDelay();

  /* Write all timestamps in the final message. See NOTE 8 below. */
  resp_msg_set_ts(&tx_resp_msg[RESP_MSG_POLL_RX_TS_IDX], poll_rx_ts);
  resp_msg_set_ts(&tx_resp_msg[RESP_MSG_RESP_TX_TS_IDX], resp_tx_ts);

  if (!serializeLocalPresence(&tx_resp_msg[RESP_MSG_PRESENCE_IDX])) {
    return false;
  }

  /* Write and send the response message. See NOTE 9 below. */
  tx_resp_msg[ALL_MSG_SN_IDX] = frameSeq_;
  if (!uwb_.transmitDelayed(tx_resp_msg, sizeof(tx_resp_msg))) {
    return false;
  }

  state_ = RangingState::SendingResponse;
  listeningEnabled_ = false;
  return true;
}

bool RangingEngine::handleResponseFrame(uint16_t frameLen) {
  /* Check that the frame is the expected response from the companion "SS TWR responder" example.
   * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
  rx_buffer[ALL_MSG_SN_IDX] = 0;
  if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) != 0) {
    Serial.println("Response header mismatch");
    return false;
  }

  // Serial.println("Response header matched");
  uint32_t poll_tx_ts, resp_rx_ts, poll_rx_ts, resp_tx_ts;
  int32_t rtd_init, rtd_resp;
  float clockOffsetRatio;

  /* Retrieve poll transmission and response reception timestamps. See NOTE 9 below. */
  poll_tx_ts = uwb_.txTimestampLo32();
  resp_rx_ts = uwb_.rxTimestampLo32();

  /* Read carrier integrator value and calculate clock offset ratio. See NOTE 11 below. */
  clockOffsetRatio = uwb_.clockOffsetRatio();

  /* Get timestamps embedded in response message. */
  resp_msg_get_ts(&rx_buffer[RESP_MSG_POLL_RX_TS_IDX], &poll_rx_ts);
  resp_msg_get_ts(&rx_buffer[RESP_MSG_RESP_TX_TS_IDX], &resp_tx_ts);

  /* Compute time of flight and distance, using clock offset ratio to correct for differing local and remote clock rates */
  rtd_init = resp_rx_ts - poll_tx_ts;
  rtd_resp = resp_tx_ts - poll_rx_ts;

  tof = ((rtd_init - rtd_resp * (1 - clockOffsetRatio)) / 2.0) * DWT_TIME_UNITS;
  distance = tof * SPEED_OF_LIGHT;
  if (distance > 0 && distance < 100) {
    if (filteredDistanceMeters_ < 0) {
      filteredDistanceMeters_ = distance;
    } else {
      filteredDistanceMeters_ = 0.7 * filteredDistanceMeters_ + 0.3 * distance;
    }
    latestDistanceMeters_ = filteredDistanceMeters_;

    if (frameLen >= RESP_MSG_PRESENCE_IDX + Protocol::PRESENCE_PACKET_SIZE) {
      const PresencePacketResult result =
          Protocol::deserialize(&rx_buffer[RESP_MSG_PRESENCE_IDX],
                                Protocol::PRESENCE_PACKET_SIZE);
      if (result.valid) {
        latestObservation_ = {
            result.packet.nodeId,
            result.packet.friendId,
            latestDistanceMeters_,
        };
        Serial.print("Observed Node ID: ");
        Serial.println(static_cast<uint8_t>(latestObservation_.nodeId));

        Serial.print("Observed Friend ID: ");
        Serial.println(static_cast<uint8_t>(latestObservation_.friendId));

        Serial.print("Observed Distance: ");
        Serial.println(latestObservation_.distanceMeters);
        return true;
      }
    }
  }

  // Serial.print("DIST: ");
  // Serial.println(distance);
  // Serial.print(" Filtered: ");
  // Serial.println(filteredDistanceMeters_);
  return false;
}

bool RangingEngine::serializeLocalPresence(uint8_t* destination) {
  const PresencePacket localPresence{
      ProtocolVersion::V1,
      localNodeId_,
      localFriendId_,
  };

  return Protocol::serialize(localPresence, destination,
                             Protocol::PRESENCE_PACKET_SIZE);
}

void RangingEngine::returnToListening() {
  state_ = RangingState::Listening;
  listeningEnabled_ = false;
}

float RangingEngine::latestDistanceMeters() const {
  return latestDistanceMeters_;
}

PresenceObservation RangingEngine::latestObservation() const {
  return latestObservation_;
}
