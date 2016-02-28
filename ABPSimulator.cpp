#include "ABPSimulator.h"
#include <iostream>
#include <queue>
#include <random>

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

ABPSimulator::ABPSimulator(unsigned int headerLength, unsigned int packetLength, unsigned int timeoutTime, unsigned int channelCapacity, unsigned int propagationDelay, double bitErrorRate) {
  this->headerLength = headerLength;
  this->packetLength = packetLength;
  this->timeoutTime = timeoutTime;
  this->channelCapacity = channelCapacity;
  this->propagationDelay = propagationDelay;
  this->bitErrorRate = bitErrorRate;

  srand(time(NULL));
}

ACKEvent* ABPSimulator::send(const double currentTime, const unsigned int sn, const unsigned int dataFrameLength) {
  unsigned int errorBits = 0;
  unsigned int ackFrameLength = this->headerLength;

  // Check if data frame sent with any errors
  for (unsigned int i = 0; i < dataFrameLength; ++i) {
    bool bitError = (((double) rand()) / RAND_MAX) < this->bitErrorRate;

    if (bitError) {
      ++errorBits;
    }

    // If frame has 5 or more error bits, consider it lost
    if (errorBits == 5) {
      return NULL;
    }
  }

  // If frame no error bits, it arrived successfully
  // Update Receiver's nextExpectedFrame
  if ((errorBits == 0) && (this->nextExpectedFrame == sn)) {
    this->nextExpectedFrame ^= 1;
  }

  errorBits = 0;

  // Check if ack frame was sent with any errors
  for (unsigned int i = 0; i < ackFrameLength; ++i) {
    bool bitError = (((double) rand()) / RAND_MAX) < this->bitErrorRate;

    if (bitError) {
      ++errorBits;
    }

    // If frame has 5 or more error bits, consider it lost
    if (errorBits == 5) {
      return NULL;
    }
  }

  ACKEvent* newAckEvent = new ACKEvent();
  newAckEvent->rn = nextExpectedFrame;
  newAckEvent->error = errorBits > 0;
  newAckEvent->time = currentTime + (1000 * ((double) (dataFrameLength + this->headerLength)) / this->channelCapacity) + (2 * this->propagationDelay);

  return newAckEvent;
}

void ABPSimulator::simulate(const unsigned int successPackets) {
  printf("ABP simulator\n");
  printf("Sender-side paramters\n");
  printf("  %-11s %d\n", "H (bits):", this->headerLength);
  printf("  %-11s %d\n", "l (bits):", this->packetLength);
  printf("  %-11s %d\n", "DELTA (ms):", this->timeoutTime);
  printf("Chanel parameters\n");
  printf("  %-11s %d\n", "C (bps):", this->channelCapacity);
  printf("  %-11s %d\n", "TAL (ms):", this->propagationDelay);
  printf("  %-11s %g\n", "BER:", this->bitErrorRate);
  printf("Experiment Duration\n");
  printf("  %-11s %d\n", "Successful Packets:", successPackets);

  const int DATA_FRAME_LENGTH = this->headerLength + this->packetLength;
  const double DATA_FRAME_TRANSMISSION_DELAY = 1000 * ((double) DATA_FRAME_LENGTH) / this->channelCapacity;

  // Sender-side
  this->sn = 0;
  this->nextExpectedAck = 1;
  double senderCurrentTime = 0.0;

  // Receiver-side
  this->nextExpectedFrame = 0;
  //double receiverCurrentTime = 0.0;

  unsigned int successPacketsDone = 0;

  TimeoutEvent *timeoutEvent = new TimeoutEvent();
  std::queue<ACKEvent *> *ackEvents = new std::queue<ACKEvent *>;

  while (successPacketsDone < successPackets) {
    timeoutEvent->time = senderCurrentTime + DATA_FRAME_TRANSMISSION_DELAY + this->timeoutTime;

    ACKEvent *newAckEvent = this->send(senderCurrentTime, this->sn, DATA_FRAME_LENGTH);
    if (newAckEvent) {
      ackEvents->push(newAckEvent);
    }

    // See if timeout occurs first or ackEvent
    if (!ackEvents->empty() && ackEvents->front()->time < timeoutEvent->time) {
      // ACK arrived before timeout

      if ((ackEvents->front()->rn == this->nextExpectedAck) && !ackEvents->front()->error) {
        this->sn ^= 1;
        this->nextExpectedAck ^= 1;
        ++successPacketsDone;
      }

      senderCurrentTime = ackEvents->front()->time;

      delete ackEvents->front();
      ackEvents->pop();
    } else {
      // Timeout before ACK
      senderCurrentTime = timeoutEvent->time;
    }
  }

  delete timeoutEvent;
  timeoutEvent = NULL;

  printf("Time to complete (ms): %f\n", senderCurrentTime);
}
