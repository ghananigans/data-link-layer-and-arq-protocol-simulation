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
  int headerLength;
  int packetLength;
  int timeoutTime;
  int channelCapacity;
  int propagationDelay;
  double bitErrorRate;

  ABPSimulator(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, double);
  void simulate(int);

private:
  // Sender-side
  unsigned int sn;
  unsigned int nextExpectedAck;

  // Receiver-side
  unsigned int nextExpectedFrame;

  ACKEvent* send(double, unsigned int, unsigned int);
};

#endif
