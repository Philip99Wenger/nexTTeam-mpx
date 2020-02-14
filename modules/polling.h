#ifndef POLLIING_H_

#define POLLING_H_

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

//! Will accept user input.  Processes input and displays the buffer.
int poll(char * buffer, int * count);

#endif
