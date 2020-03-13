#ifndef R3COMMANDS_H_

#define R3COMMAND_H_

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "pcb.h"
#include "procsr3.h"
#include "context.h"
#include "commandhandler.h"

//! makes command handler yield to other processes
void yield();
//! loads 1 process at a time
void load(char * name, void (*procfunc)(), int priority);
//! loads processes in given proc3.c file as suspend-ready
void loadr3();
#endif


