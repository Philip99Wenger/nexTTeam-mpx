#ifndef R4COMMANDS_H_

#define R4COMMAND_H_

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "r3commands.h"
#include "mpx_supt.h"

//! Initializes an Alarm PCB
void setAlarm(char * timeStatement, char * message);
//! Checks to see if it has passed a certain time
void alarmProcess();
//! Idles indefinitely
void infinite();
#endif

