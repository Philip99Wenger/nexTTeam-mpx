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
#include "r3commands.h"
#include "r4commands.h"
#include "mcb.h"

//int poll(char * buffer, int * count);
//! Displays the current version being used for the MPX
void version();
//! Displays instructions for how to use each command
void help();
void help1();
void help2();
//! Shutdown the MPX and terminate
int shutdown();
//! Prompts the user for time input and sets the time
void settimeWrapper();
//! Prompts the user for date input and sets the date
void setdateWrapper();
//! Prompts the user for alarm input and sets an alarm
void setalarmWrapper();
//! Creates a process that sits idle forever
void infiniteWrapper();
//! Processes user input and find the correct function based off user request
void comhand();
//! Allows the user to create a process
//void createPCBWrapper();
//! Allows the user to remove a process
//void deletePCBWrapper();
//! Prompts the user for the name and priority value
void setPriorityWrapper();
//! Prompts the user for a PCB name
void showPCBWrapper();
//! Prompts the user for a PCB name (and finds that pcb to verify its valid)
//void blockWrapper();
//! Prompts the user for a PCB name (and finds that pcb to verify its valid)
//void unblockWrapper();
//! Initializes the heap and asks for size
void initializeHeapWrapper();
//! Checks to see if the heap is empty
void isEmpty();
//! Displays the previous ten commands used by the user
void historyWrapper();
//! Prompts the user for a PCB name than sets it to suspended
void suspendWrapper();
//! Prompts the user for a PCB name than sets it to not suspended
void resumeWrapper();

#endif

