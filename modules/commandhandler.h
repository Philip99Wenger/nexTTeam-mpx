#ifndef COMMANDHANDLER_H_

#define COMMANDHANDLER_H_

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
#include "date.h"
#include "time.h"

//int poll(char * buffer, int * count);
void version();
void help();

#endif
