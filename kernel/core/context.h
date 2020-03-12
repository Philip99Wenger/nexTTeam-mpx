#ifndef _CONTEXT_H
#define _CONTEXT_H

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
<<<<<<< HEAD
#include "pcb.h"
=======
>>>>>>> 21ec8a7f158fdb4c98e46b990bd2c6a68a337328

typedef struct context{
	u32int gs, fs, es, ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int eip, cs, eflags;
}

u32int* sys_call(context *registers);

#endif
