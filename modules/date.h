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
#include "time.h"

//! Get the date from the registers.  Display the date.
void getdate();
//! Set the date in the registers. User supplies the month, day, and year to set.
void setDate(int month, int day, int year);

#endif
