CC = g++ -std=c++11
CFLAGS = -g -Wall
OBJECT_DIR = obj

.PHONY: default all clean

default: generateABPCSV generateABPNAKCSV generateGBNCSV
all: default

HEADERS = $(wildcard *.h)
ABPSIMULATOR_DEBUG_OBJECTS = $(OBJECT_DIR)/ABPSimulator_debug.o $(OBJECT_DIR)/ABPSimulator.o
GBNSIMULATOR_DEBUG_OBJECTS= $(OBJECT_DIR)/GBNSimulator_debug.o $(OBJECT_DIR)/GBNSimulator.o
GENERATE_ABP_CSV_OBJECTS = $(OBJECT_DIR)/generateABPCSV.o $(OBJECT_DIR)/ABPSimulator.o
GENERATE_ABP_NAK_CSV_OBJECTS = $(OBJECT_DIR)/generateABPNAKCSV.o $(OBJECT_DIR)/ABPSimulator.o
GENERATE_GBN_CSV_OBJECTS = $(OBJECT_DIR)/generateGBNCSV.o $(OBJECT_DIR)/GBNSimulator.o

$(OBJECT_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

ABPSimulator_debug: $(ABPSIMULATOR_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(ABPSIMULATOR_DEBUG_OBJECTS) -o $@

GBNSimulator_debug: $(GBNSIMULATOR_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(GBNSIMULATOR_DEBUG_OBJECTS) -o $@

generateABPCSV: $(GENERATE_ABP_CSV_OBJECTS)
	$(CC) $(CFLAGS) $(GENERATE_ABP_CSV_OBJECTS) -o $@

generateABPNAKCSV: $(GENERATE_ABP_NAK_CSV_OBJECTS)
	$(CC) $(CFLAGS) $(GENERATE_ABP_NAK_CSV_OBJECTS) -o $@

generateGBNCSV: $(GENERATE_GBN_CSV_OBJECTS)
	$(CC) $(CFLAGS) $(GENERATE_GBN_CSV_OBJECTS) -o $@

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f generateABPCSV
	-rm -f generateABPNAKCSV
	-rm -f generateGBNCSV
	-rm -f ABPSimulator_debug
	-rm -f GBNSimulator_debug
