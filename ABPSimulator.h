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
  unsigned int timeoutTime;
  unsigned int channelCapacity;
  unsigned int propagationDelay;
  double bitErrorRate;
  bool ackNak;

  ABPSimulator(bool, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, double);
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
