LIBS = -lm -lueye_api -lzmq
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

capture: capture.o camera.o
		$(CC) $^ -Wall $(LIBS) -o $@

bench: bench.o camera.o
		$(CC) $^ -Wall $(LIBS) -o $@

client: client.o
		$(CC) $^ -Wall $(LIBS) -o $@

all: capture bench client

clean:
		-rm -f *.o
		-rm -f capture
		-rm -f bench
		-rm -f client


