#include "RangingEngine.h"

#include "../protocol/Protocol.h"
#include "Identity.h"
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

static uint8_t tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0xE0, 0, 0, 0, 0, 0};
static uint8_t rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t frame_seq_nb = 0;
static uint8_t rx_buffer[23];
static double tof;
static double distance;

RangingEngine::RangingEngine(UWBManager& uwb) : uwb_(uwb) {}

bool RangingEngine::update() {
  const uint32_t nowMs = millis();
  if (nowMs - lastRangeAttemptMs_ < RNG_DELAY_MS) {
    return false;
  }
  lastRangeAttemptMs_ = nowMs;
  bool validObservationCalculated = false;
  const PresencePacket localPresence{
      ProtocolVersion::V1,
      MY_NODE_ID,
      MY_FRIEND,
  };

  if (!Protocol::serialize(localPresence, &tx_poll_msg[POLL_MSG_PRESENCE_IDX],
                           Protocol::PRESENCE_PACKET_SIZE)) {
    return false;
  }

  /* Write frame data to DW IC and prepare transmission. See NOTE 7 below. */
  tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
  uwb_.transmitAndExpectResponse(tx_poll_msg, sizeof(tx_poll_msg));

  const UWBReceiveStatus receiveStatus = uwb_.waitForReceive();

  /* Increment frame sequence number after transmission of the poll message (modulo 256). */
  frame_seq_nb++;

  if (receiveStatus == UWBReceiveStatus::FrameReceived) {
    uint32_t frame_len;

    /* Clear good RX frame event in the DW IC status register. */
    uwb_.clearGoodReceive();

    /* A frame has been received, read it into the local buffer. */
    frame_len = uwb_.receivedFrameLength();
    if (frame_len <= sizeof(rx_buffer)) {
      uwb_.readReceivedFrame(rx_buffer, frame_len);

      /* Check that the frame is the expected response from the companion "SS TWR responder" example.
       * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
      rx_buffer[ALL_MSG_SN_IDX] = 0;
      if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0) {
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

          if (frame_len >= RESP_MSG_PRESENCE_IDX + Protocol::PRESENCE_PACKET_SIZE) {
            const PresencePacketResult result =
                Protocol::deserialize(&rx_buffer[RESP_MSG_PRESENCE_IDX],
                                      Protocol::PRESENCE_PACKET_SIZE);
            if (result.valid) {
              latestObservation_ = {
                  result.packet.nodeId,
                  result.packet.friendId,
                  latestDistanceMeters_,
              };
              validObservationCalculated = true;
              Serial.print("Observed Node ID: ");
              Serial.println(static_cast<uint8_t>(latestObservation_.nodeId));

              Serial.print("Observed Friend ID: ");
              Serial.println(static_cast<uint8_t>(latestObservation_.friendId));

              Serial.print("Observed Distance: ");
              Serial.println(latestObservation_.distanceMeters);
            }
          }
        }
        // Serial.print("DIST: ");
        // Serial.println(distance);
        // Serial.print(" Filtered: ");
        // Serial.println(filteredDistanceMeters_);
      } else {
        Serial.println("Response header mismatch");
      }
    }
  } else {
    /* Clear RX error/timeout events in the DW IC status register. */
    // dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
    // Serial.print("RX timeout/error. Status = 0x");
    // Serial.println(status_reg, HEX);
    uwb_.clearReceiveTimeoutOrError();
  }

  return validObservationCalculated;
}

float RangingEngine::latestDistanceMeters() const {
  return latestDistanceMeters_;
}

PresenceObservation RangingEngine::latestObservation() const {
  return latestObservation_;
}
