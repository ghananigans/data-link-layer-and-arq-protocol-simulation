CC = g++ -std=c++11
CFLAGS = -g -Wall
OBJECT_DIR = obj

.PHONY: default all clean

default: lab2
all: default

HEADERS = $(wildcard *.h)
LAB2_OBJECTS = $(OBJECT_DIR)/lab2.o $(OBJECT_DIR)/ABPSimulator.o
ABPSIMULATOR_DEBUG_OBJECTS = $(OBJECT_DIR)/ABPSimulator_debug.o $(OBJECT_DIR)/ABPSimulator.o
GBNSIMULATOR_DEBUG_OBJECTS= $(OBJECT_DIR)/GBNSimulator_debug.o $(OBJECT_DIR)/GBNSimulator.o

$(OBJECT_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

lab2: $(LAB2_OBJECTS)
	$(CC) $(CFLAGS) $(LAB2_OBJECTS) -o $@

ABPSimulator_debug: $(ABPSIMULATOR_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(ABPSIMULATOR_DEBUG_OBJECTS) -o $@

GBNSimulator_debug: $(GBNSIMULATOR_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(GBNSIMULATOR_DEBUG_OBJECTS) -o $@

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f lab2
	-rm -f ABPSimulator_debug
