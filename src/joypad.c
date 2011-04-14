#include "joypad.h"
#include "mmu.h"

struct Joypad joypad = {0, 0xF, 0xF};

void joypad_reset()
{
    joypad.column = 0;
    joypad.dpad   = 0xF;
    joypad.button = 0xF;
}

Byte joypad_getbyte()
{
    switch (joypad.column)
    {
        case COLUMN_DPAD:
            return joypad.dpad;

        case COLUMN_BUTTON:
            return joypad.button;

        default:
            return 0;
    }
}

void joypad_putbyte(Byte value)
{
    joypad.column = value & 0x30;
}

void joypad_keydown(SDLKey key)
{
    switch (key)
    {
        case SDLK_DOWN:
            joypad.dpad &= ~DPAD_DOWN;
            break;

        case SDLK_UP:
            joypad.dpad &= ~DPAD_UP;
            break;

        case SDLK_LEFT:
            joypad.dpad &= ~DPAD_LEFT;
            break;

        case SDLK_RIGHT:
            joypad.dpad &= ~DPAD_RIGHT;
            break;

        case SDLK_RETURN:
            joypad.button &= ~BUTTON_START;
            break;

        case SDLK_SPACE:
            joypad.button &= ~BUTTON_SELECT;
            break;

        case SDLK_x:
            joypad.button &= ~BUTTON_B;
            break;

        case SDLK_z:
            joypad.button &= ~BUTTON_A;
            break;

        default:
            break;
    }

    mmu.iflag |= INT_JOYPAD;
}

void joypad_keyup(SDLKey key)
{
    switch (key)
    {
        case SDLK_DOWN:
            joypad.dpad |= DPAD_DOWN;
            break;

        case SDLK_UP:
            joypad.dpad |= DPAD_UP;
            break;

        case SDLK_LEFT:
            joypad.dpad |= DPAD_LEFT;
            break;

        case SDLK_RIGHT:
            joypad.dpad |= DPAD_RIGHT;
            break;

        case SDLK_RETURN:
            joypad.button |= BUTTON_START;
            break;

        case SDLK_SPACE:
            joypad.button |= BUTTON_SELECT;
            break;

        case SDLK_x:
            joypad.button |= BUTTON_B;
            break;

        case SDLK_z:
            joypad.button |= BUTTON_A;
            break;

        default:
            break;
    }

    mmu.iflag |= INT_JOYPAD;
}

