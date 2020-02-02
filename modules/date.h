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

void getdate();
int bcdToInt1(unsigned char value);
void setDate(int month, int day, int year);
unsigned char intToBcd1(int data);

#endif
