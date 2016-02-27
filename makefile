CC = g++ -std=c++11
CFLAGS = -g -Wall
OBJECT_DIR = obj

.PHONY: default all clean

default: lab2
all: default

HEADERS = $(wildcard *.h)
LAB2_OBJECTS = $(OBJECT_DIR)/lab2.o $(OBJECT_DIR)/ABPSimulator.o
LAB2_DEBUG_OBJECTS = $(OBJECT_DIR)/lab2_debug.o

$(OBJECT_DIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

lab2: $(LAB2_OBJECTS)
	$(CC) $(CFLAGS) $(LAB2_OBJECTS) -o $@

lab2_debug: $(LAB2_DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(LAB2_DEBUG_OBJECTS) -o $@

clean:
	-rm -f $(OBJECT_DIR)/*.o
	-rm -f lab2
	-rm -f lab2_debug
