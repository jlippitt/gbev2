CC=gcc
CFLAGS=-std=c99 -Wall
LDFLAGS=-Wall
LDLIBS=
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gbe.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm src/*.o $(EXECUTABLE)

