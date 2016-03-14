#include <iostream>
#include <fstream>
#include "ABPSimulator.h"

int main(int argc, char *argv[]) {
  printf("ABPNAKSimulator\n");

  unsigned int successPackets = 10000;
	unsigned int H =  54 * 8; // Bits
	unsigned int l = 1500 * 8; // Bits
	unsigned int C = 5 * 1024 * 1024; // Bits per second (bps)
  bool ackNak = true;

  double TAL = 5; // Milliseconds
  double DELTA = TAL * 2.5; // Milliseconds
  double BER = 0.0;

  ABPSimulator abpsimulator(ackNak, H, l, DELTA, C, TAL, BER);
  double factor;
  double throughput;
  std::ofstream csvFile;

  csvFile.open("ABP_NAK.csv");

  for (int i = 1; i <= 5; ++i) {
    factor = 2.5 * i;

    TAL = 5;
    DELTA = TAL * factor;
    BER = 0.0;
    abpsimulator.timeoutTime = DELTA;
    abpsimulator.propagationDelay = TAL;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-5;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-4;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    TAL = 250;
    DELTA = TAL * factor;
    BER = 0.0;
    abpsimulator.timeoutTime = DELTA;
    abpsimulator.propagationDelay = TAL;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-5;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-4;
    abpsimulator.bitErrorRate = BER;
    throughput = abpsimulator.simulate(successPackets);
    csvFile << throughput << "\n";
  }

  csvFile.close();

	exit(EXIT_SUCCESS);
}
