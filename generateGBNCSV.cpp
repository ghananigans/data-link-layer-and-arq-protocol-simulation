#include <iostream>
#include <fstream>
#include "GBNSimulator.h"

int main(int argc, char *argv[]) {
  printf("GBNSimulator\n");

  unsigned int successPackets = 10000;
	unsigned int H =  54 * 8; // Bits
	unsigned int l = 1500 * 8; // Bits
	unsigned int C = 5 * 1024 * 1024; // Bits per second (bps)
  unsigned int bufferSize = 4;

  double TAL = 5; // Milliseconds
  double DELTA = TAL * 2.5; // Milliseconds
  double BER = 0.0;

  GBNSimulator gbnsimulator(H, l, DELTA, C, TAL, bufferSize, BER);
  double factor;
  double throughput;
  std::ofstream csvFile;

  csvFile.open("GBN.csv");

  for (int i = 1; i <= 5; ++i) {
    factor = 2.5 * i;

    TAL = 5;
    DELTA = TAL * factor;
    BER = 0.0;
    gbnsimulator.timeoutTime = DELTA;
    gbnsimulator.propagationDelay = TAL;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-5;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-4;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    TAL = 250;
    DELTA = TAL * factor;
    BER = 0.0;
    gbnsimulator.timeoutTime = DELTA;
    gbnsimulator.propagationDelay = TAL;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-5;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << ",";

    BER = 1e-4;
    gbnsimulator.bitErrorRate = BER;
    throughput = gbnsimulator.simulate(successPackets);
    csvFile << throughput << "\n";
  }

  csvFile.close();

	exit(EXIT_SUCCESS);
}
