CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror
LDFLAGS=-Wall
LDLIBS=-lSDLmain -lSDL
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gbe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm src/*.o $(EXECUTABLE)

