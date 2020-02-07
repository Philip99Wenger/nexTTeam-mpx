#ifndef TIME_H_

#define TIME_H_

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "mpx_supt.h"

//! Will get the time.  Displays the time.
void gettime();
//! Accepts an unsigned char and converts from BCD to an integer
int bcdToInt(unsigned char value);
//! Sets the time based off user input.
void setTime(int hours, int minutes, int seconds);
//! Accepts an integer and converts from an integer to a BCD
unsigned char intToBcd(int data);
//! Accepts an integer and converts from an integer to ASCII
char * intToAscii(int integer);

#endif
