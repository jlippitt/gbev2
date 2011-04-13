#ifndef JOYPAD_H
#define JOYPAD_H

#include <SDL/SDL.h>
#include "types.h"

#define COLUMN_DPAD   0x20
#define COLUMN_BUTTON 0x10

#define DPAD_DOWN     0x08
#define DPAD_UP       0x04
#define DPAD_LEFT     0x02
#define DPAD_RIGHT    0x01

#define BUTTON_START  0x08
#define BUTTON_SELECT 0x04
#define BUTTON_B      0x02
#define BUTTON_A      0x01

extern struct Joypad
{
    Byte column;
    Byte dpad;
    Byte button;
}
joypad;

void joypad_reset();

Byte joypad_getbyte();

void joypad_putbyte(Byte value);

void joypad_keydown(SDLKey key);

void joypad_keyup(SDLKey key);

#endif
