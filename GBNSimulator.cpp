#include "GBNSimulator.h"
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

GBNSimulator::GBNSimulator(unsigned int headerLength, unsigned int packetLength, double timeoutTime, unsigned int channelCapacity, double propagationDelay, unsigned int bufferSize, double bitErrorRate) {
  this->headerLength = headerLength;
  this->packetLength = packetLength;
  this->timeoutTime = timeoutTime;
  this->channelCapacity = channelCapacity;
  this->propagationDelay = propagationDelay;
  this->bufferSize = bufferSize;
  this->bitErrorRate = bitErrorRate;

  srand(time(NULL));
}

ACKEvent* GBNSimulator::send(const double currentTime, const unsigned int sn, const unsigned int dataFrameLength) {
  unsigned int errorBits = 0;
  unsigned int ackFrameLength = this->headerLength;

  // Check if data frame sent with any errors
  for (unsigned int i = 0; i < dataFrameLength; ++i) {
    bool bitError = (((double) rand()) / RAND_MAX) < this->bitErrorRate;

    if (bitError && (++errorBits == 5)) {
      // If frame has 5 or more error bits, consider it lost
      return NULL;
    }
  }

  // If frame no error bits, it arrived successfully
  // Update Receiver's nextExpectedFrame
  if ((errorBits == 0) && (this->nextExpectedFrame == sn)) {
    this->nextExpectedFrame = (this->nextExpectedFrame + 1) % (this->bufferSize + 1);
  }

  errorBits = 0;

  // Check if ack frame was sent with any errors
  for (unsigned int i = 0; i < ackFrameLength; ++i) {
    bool bitError = (((double) rand()) / RAND_MAX) < this->bitErrorRate;

    if (bitError && (++errorBits == 5)) {
      // If frame has 5 or more error bits, consider it lost
      return NULL;
    }
  }

  ACKEvent* newAckEvent = new ACKEvent();
  newAckEvent->rn = this->nextExpectedFrame;
  newAckEvent->error = errorBits > 0;
  newAckEvent->time = currentTime + (1000 * ((double) (dataFrameLength + ackFrameLength)) / this->channelCapacity) + (2 * this->propagationDelay);

  return newAckEvent;
}

void GBNSimulator::simulate(const unsigned int successPackets) {
  printf("ABP simulator\n");
  printf("Sender-side paramters\n");
  printf("  %-11s %d\n", "H (bits):", this->headerLength);
  printf("  %-11s %d\n", "l (bits):", this->packetLength);
  printf("  %-11s %f\n", "DELTA (ms):", this->timeoutTime);
  printf("  %-11s %d\n", "Buffer Size:", this->bufferSize);
  printf("Chanel parameters\n");
  printf("  %-11s %d\n", "C (bps):", this->channelCapacity);
  printf("  %-11s %f\n", "TAL (ms):", this->propagationDelay);
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

  TimeoutEvent *timeoutEvent = NULL;
  std::queue<ACKEvent *> *ackEvents = new std::queue<ACKEvent *>;
  std::queue<Packet *> *buffer = new std::queue<Packet *>;

  while (successPacketsDone < successPackets) {
    double senderNextTime = 0.0;

    if ((this->bufferSize - buffer->size()) > 0) {
      // buffer can fit more stuff now
      Packet *newPacket = new Packet();
      newPacket->sn = this->sn;
      senderNextTime = senderCurrentTime + DATA_FRAME_TRANSMISSION_DELAY;
      newPacket->timeSent = senderNextTime;

      buffer->push(newPacket);

      ACKEvent *newAckEvent = this->send(senderCurrentTime, this->sn, DATA_FRAME_LENGTH);
      if (newAckEvent) {
        ackEvents->push(newAckEvent);
      }

      this->sn = (this->sn + 1) % (this->bufferSize + 1);

      if (!timeoutEvent) {
        timeoutEvent = new TimeoutEvent;
        timeoutEvent->time = senderNextTime + this->timeoutTime;
      }
    } else {
      if (ackEvents->empty() || (timeoutEvent->time <= ackEvents->front()->time)) {
        senderNextTime = timeoutEvent->time;
      } else {
        senderNextTime = ackEvents->front()->time;
      }
    }

    if (ackEvents->empty() || ((timeoutEvent->time <= senderNextTime) && (timeoutEvent->time <= ackEvents->front()->time))) {
      // Timeout occured
      delete timeoutEvent;
      timeoutEvent = NULL;

      // need to resent everything in buffer so just dump
      // buffer and recreate/send
      while (!buffer->empty()) {
        this->sn = (this->sn - 1) % (this->bufferSize + 1);
        buffer->pop();
      }
    }

    // Pop all ACK Events that occured already
    while(!ackEvents->empty() && (ackEvents->front()->time <= senderNextTime)) {
      if (!buffer->empty() && !ackEvents->front()->error && (buffer->front()->sn != ackEvents->front()->rn)) {
        for (int i = (ackEvents->front()->rn - buffer->front()->sn) % (this->bufferSize + 1); i > 0; --i) {
          ++successPacketsDone;

          delete buffer->front();
          buffer->pop();

          if (timeoutEvent) {
            delete timeoutEvent;
            timeoutEvent = NULL;
          }
        }
      }
      /*
      while (!buffer->empty() && !ackEvents->front()->error && (buffer->front()->sn < ackEvents->front()->rn)) {
        ++successPacketsDone;

        delete buffer->front();
        buffer->pop();

        if (timeoutEvent) {
          delete timeoutEvent;
          timeoutEvent = NULL;
        }
      }*/

      delete ackEvents->front();
      ackEvents->pop();
    }

    senderCurrentTime = senderNextTime;
  }

  if (timeoutEvent) {
    delete timeoutEvent;
    timeoutEvent = NULL;
  }

  // Delete ackEvents & buffer queues
  delete ackEvents;
  delete buffer;

  buffer = NULL;
  ackEvents = NULL;

  unsigned int totalBitsSent = successPacketsDone * this->packetLength;
  double throughput = totalBitsSent / (senderCurrentTime / 1000);

  printf("Time to complete (ms): %f\n", senderCurrentTime);
  printf("Throughput (bps): %f\n", throughput);
}
