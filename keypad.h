#ifndef __keypad__
#define __keypad__

#include "avr.h"

unsigned int keypad_is_pressed(int r, int c);
unsigned int keypad_get_key();

#endif