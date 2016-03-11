#ifndef _ABPSIMULATOR_H_
#define _ABPSIMULATOR_H_

struct ACKEvent {
  unsigned int rn;
  bool error;
  double time;
};

struct TimeoutEvent {
  double time;
};

class ABPSimulator {
public:
  unsigned int headerLength;
  unsigned int packetLength;
  double timeoutTime;
  unsigned int channelCapacity;
  double propagationDelay;
  double bitErrorRate;
  bool ackNak;

  ABPSimulator(bool, unsigned int, unsigned int, double, unsigned int, double, double);
  void simulate(unsigned int);

private:
  // Sender-side
  unsigned int sn;
  unsigned int nextExpectedAck;
  // Receiver-side
  unsigned int nextExpectedFrame;

  ACKEvent* send(double, unsigned int, unsigned int);
};

#endif
