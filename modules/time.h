#ifndef DATE_H_

#define DATE_H_

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

void gettime();
int bcdToInt(unsigned char value);
void setTime(int month, int day, int year);
unsigned char intToBcd(int data);
char * intToAscii(int integer);

#endif
