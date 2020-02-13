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
#include "pcb.h"

//int poll(char * buffer, int * count);
//! Displays the current version being used for the MPX
void version();
//! Displays instructions for how to use each command
void help();
//! Shutdown the MPX and terminate
int shutdown();
//! Prompts the user for time input and sets the time
void settimeWrapper();
//! Prompts the user for date input and sets the date
void setdateWrapper();
//! Processes user input and find the correct function based off user request
int comhand();
//! Allows the user to create a process
void createPCB();
//! Allows the user to remove a process
void deletePCB();

#endif
