TARGET = capture
LIBS = -lm -lueye_api -lzmq
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

SRCS = capture.c camera.c
OBJECTS = $(patsubst %.c, %.o, $(SRCS))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

bench: bench.o camera.o
		$(CC) $^ -Wall $(LIBS) -o $@

clean:
		-rm -f $(OBJECTS)
		-rm -f $(TARGET)
		-rm -f bench

