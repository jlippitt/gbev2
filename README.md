# Compilation

## Ubuntu and derivatives

    sudo apt-get install build-essential libsdl-dev libsdl-image1.2-dev
    make

## Windows (MinGW)

You will need to have MinGW, SDL 1.2 and SDL Image installed and available in the include path.

On Windows, it's necessary to link against the MinGW library. In Makefile, change:

    LDLIBS=-lSDLmain -lSDL

to

    LDLIBS=-lmingw32 -lSDLmain -lSDL

Then run make as normal:

    make

# Usage

    ./gbe <rom-file>

# Game Controls

* Directional buttons = D-Pad
* Enter = Start button
* Space = Select button
* Z = A button
* X = B button

