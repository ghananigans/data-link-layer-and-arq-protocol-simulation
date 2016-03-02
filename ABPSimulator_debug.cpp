#include <iostream>
#include <unistd.h>
#include "ABPSimulator.h"

using namespace std;

int main(int argc, char *argv[]) {
  printf("ABPSimulator Debug");

  unsigned int successPackets = 50;
	unsigned int H =  54 * 8; // Bits
	unsigned int l = 1500 * 8; // Bits
	unsigned int DELTA = 25; // Milliseconds
	unsigned int C = 5 * 1024 * 1024; // Bits per second (bps)
	unsigned int TAL = 10; // Milliseconds
	double BER = 0.0;
  bool ackNak = false;

	int c;
	while ((c = getopt(argc, argv, "hNH:l:D:C:T:B:S:")) != -1) {
		switch (c) {
			case 'H':
				H = (unsigned int) std::stoi(optarg);
				break;
			case 'l':
				l = (unsigned int) std::stoi(optarg);
				break;
			case 'D':
				DELTA = (unsigned int) std::stoi(optarg);
				break;
			case 'C':
				C = (unsigned int) std::stoi(optarg);
				break;
			case 'T':
				TAL = (unsigned int) std::stoi(optarg);
				break;
			case 'B':
				BER = std::stod(optarg);
				break;
      case 'S':
  			successPackets = (unsigned int) std::stoi(optarg);
  			break;
      case 'N':
        ackNak = true;
        break;
			case 'h':
			case '?':
				printf("ABP Simulator Debug\n\n");
				printf("    Parameters:\n");
				printf("%8s%-10s %s\n", "", "-H <arg>", "Header length (bits)");
				printf("%8s%-10s %s\n", "", "-l <arg>", "Packet length (bits)");
				printf("%8s%-10s %s\n", "", "-D <arg>", "Timeout time (ms)");
				printf("%8s%-10s %s\n", "", "-C <arg>", "Channel capacity (bps)");
				printf("%8s%-10s %s\n", "", "-T <arg>", "Propagation delay (ms)");
				printf("%8s%-10s %s\n", "", "-B <arg>", "Bit error rate");
				printf("%8s%-10s %s\n", "", "-S <arg>", "Successful packets to simulate");
				printf("%8s%-10s %s\n", "", "-h", "Help");
				printf("\n");
				exit(EXIT_SUCCESS);
		}
	}

  ABPSimulator abpsimulator(ackNak, H, l, DELTA, C, TAL, BER);
  abpsimulator.simulate(successPackets);

	exit(EXIT_SUCCESS);
}
