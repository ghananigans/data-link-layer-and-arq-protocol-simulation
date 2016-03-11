#include <iostream>
#include <unistd.h>
#include "GBNSimulator.h"

using namespace std;

int main(int argc, char *argv[]) {
  printf("GBNSimulator Debug");

  unsigned int successPackets = 50;
	unsigned int H =  54 * 8; // Bits
	unsigned int l = 1500 * 8; // Bits
  double DELTA = 25; // Milliseconds
	unsigned int C = 5 * 1024 * 1024; // Bits per second (bps)
	double TAL = 10; // Milliseconds
  unsigned int bufferSize = 4;
	double BER = 0.0;

	int c;
	while ((c = getopt(argc, argv, "hH:l:D:C:T:B:S:Z:")) != -1) {
		switch (c) {
			case 'H':
				H = (unsigned int) std::stoi(optarg);
				break;
			case 'l':
				l = (unsigned int) std::stoi(optarg);
				break;
			case 'D':
				DELTA = std::stod(optarg);
				break;
			case 'C':
				C = (unsigned int) std::stoi(optarg);
				break;
			case 'T':
				TAL = std::stod(optarg);
				break;
			case 'B':
				BER = std::stod(optarg);
				break;
      case 'S':
  			successPackets = (unsigned int) std::stoi(optarg);
  			break;
      case 'Z':
  			bufferSize = (unsigned int) std::stoi(optarg);
  			break;
			case 'h':
			case '?':
				printf("GBN Simulator Debug\n\n");
				printf("    Parameters:\n");
				printf("%8s%-10s %s\n", "", "-H <arg>", "Header length (bits)");
				printf("%8s%-10s %s\n", "", "-l <arg>", "Packet length (bits)");
				printf("%8s%-10s %s\n", "", "-D <arg>", "Timeout time (ms)");
				printf("%8s%-10s %s\n", "", "-C <arg>", "Channel capacity (bps)");
				printf("%8s%-10s %s\n", "", "-T <arg>", "Propagation delay (ms)");
				printf("%8s%-10s %s\n", "", "-B <arg>", "Bit error rate");
				printf("%8s%-10s %s\n", "", "-S <arg>", "Successful packets to simulate");
        printf("%8s%-10s %s\n", "", "-Z <arg>", "Buffer Size");
				printf("%8s%-10s %s\n", "", "-h", "Help");
				printf("\n");
				exit(EXIT_SUCCESS);
		}
	}

  GBNSimulator gbnsimulator(H, l, DELTA, C, TAL, bufferSize, BER);
  gbnsimulator.simulate(successPackets);

	exit(EXIT_SUCCESS);
}
