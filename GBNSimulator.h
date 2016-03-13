#ifndef _GBNSIMULATOR_H_
#define _GBNSIMULATOR_H_

struct ACKEvent {
  unsigned int rn;
  bool error;
  double time;
};

struct TimeoutEvent {
  double time;
};

struct Packet {
  unsigned int sn;
  double timeSent;
};

class GBNSimulator {
public:
  unsigned int headerLength;
  unsigned int packetLength;
  double timeoutTime;
  unsigned int channelCapacity;
  double propagationDelay;
  unsigned int bufferSize;
  double bitErrorRate;

  GBNSimulator(unsigned int, unsigned int, double, unsigned int, double, unsigned int, double);
  double simulate(unsigned int);

private:
  // Sender-side
  unsigned int sn;
  unsigned int nextExpectedAck;
  // Receiver-side
  unsigned int nextExpectedFrame;

  ACKEvent* send(double, unsigned int, unsigned int);
};

#endif
