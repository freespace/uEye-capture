TARGET = capture
LIBS = -lm -lueye_api
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

SRCS = capture.c
OBJECTS = $(patsubst %.c, %.o, $(SRCS))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
		-rm -f *.o
		-rm -f $(TARGET)

