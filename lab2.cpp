#include <iostream>
#include "ABPSimulator.h"

using namespace std;

int main(int argc, char *argv[]) {
  printf("Lab 2\n");

  const int SUCCESS_PACKETS = 50; // Successfully delievered packets
  const int HEADER_LENGTH = 54 * 8; // Bits
  const int PACKET_LENGTH = 1500 * 8; // Bits
  const int TIMEOUT_TIME = 25; // Milliseconds
  const int CHANNEL_CAPACITY = 5 * 1024 * 1024; // Bits per second (bps)
  const int PROPAGATION_DELAY = 10; // Milliseconds
  const double BIT_ERROR_RATE = 0.0000214;
  const bool ackNak = true;

  ABPSimulator abpsimulator(ackNak, HEADER_LENGTH, PACKET_LENGTH, TIMEOUT_TIME, CHANNEL_CAPACITY, PROPAGATION_DELAY, BIT_ERROR_RATE);
  abpsimulator.simulate(SUCCESS_PACKETS);
	exit(EXIT_SUCCESS);
}
