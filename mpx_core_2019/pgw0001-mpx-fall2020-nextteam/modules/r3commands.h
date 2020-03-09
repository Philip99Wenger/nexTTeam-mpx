#i#ifndef R3COMMANDS_H_

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

//! makes command handler yield to other processes
void yield();
//! loads processes in given proc3.c file as suspend-ready
void loadr3();
#endif

