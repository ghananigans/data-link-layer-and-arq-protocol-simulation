#include "ABPSimulator.h"
#include <iostream>
#include <queue>

using namespace std;


/**
 *  Sender-side parameters
 *  H [HEADER_LENGTH]:      Frame header length (bits)
 *  l [PACKET_LENGTH]:      Packet length (bits)
 *  Δ [TIMEOUT_TIME]:       Timeout (milliseconds)
 *
 *  Channel parameters
 *  C [CHANNEL_CAPACITY]:   Channel bitrate (bps)
 *  τ [PROPAGATION_DELAY]:  Propagation delay (milliseconds)
 *  BER [BIT_ERROR_RATE]:   Bit error rate
 *
 * Experiment duration in terms of number of successfully delivered packets to be simulated
 */

struct Packet {
  unsigned int SN;
};

struct ACKEvent {
  unsigned int RN;
  bool error;
};

struct TimeOutEvent {
  double time;
};

void simulator(const int SUCCESS_PACKETS, const int HEADER_LENGTH, const int PACKET_LENGTH, const int TIMEOUT_TIME, const int CHANNEL_CAPACITY, const int PROPAGATION_DELAY, const double BIT_ERROR_RATE) {
  const int FRAME_LENGTH = HEADER_LENGTH + PACKET_LENGTH;
  // Packet and Ack transmission delay time in milli seconds
  const double FRAME_TRANSMISSION_DELAY = 1000 * ((double) FRAME_LENGTH) / CHANNEL_CAPACITY;
  const double ACK_TRANSMISSION_DELAY = 1000 * ((double) HEADER_LENGTH) / CHANNEL_CAPACITY;

  // Sender-side
  unsigned int sn = 0;
  unsigned int nextExpectedAck = 1;
  double timeOutTime = 0.0;
  double senderCurrentTime = 0.0;

  // Receiver-side
  unsigned int nextExpectedFreame = 0;
  double receiverCurrentTime = 0.0;

  TimeOutEvent *timeOutEvent = NULL;
  ACKEvent *ackEvent = NULL;

  printf("simulator\n");
}
