#ifndef PCB_H_

#define PCB_H_

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define SUSPENED 1
#define NOTSUSPENDED 0

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

typedef struct pcb{ 
	char processName[8];		//The name of the process
	char *namePtr;	//Pointer to the name
	int priority;			//0-9 ranking of importance, with 9 going first
	int stateRRB;			//Ready(0)/Running(1)/Blocked(2)
	int stateIsSuspended;		//Suspended(1)/Not-Suspended(0)
	int classIsApp;			//Application(1)/System-Process(0)
	struct pcb* pcbNext;		//Pointer to the start of the next process
	struct pcb* pcbPrev;		//Pointer to the start of the last process

	//This is the stack//	
	unsigned char* base;		//Pointer to the base of the process
	unsigned char* top;		//Pointer to the top of the process, or base + 1024 bytes
	//1024 Bytes, Every Character should be initialized to null//
} pcb;

typedef struct Queue{
	int count;	//Number of processes in the queue
	pcb* head;	//Pointer to the first pcb in the queue, the first one in order
	pcb* tail;	//Pointer to the last pcb in the queue, the last one in order
}Queue;

//! Inserts PCB into the correct queue
void insertPCB(pcb* Pcb);

pcb* allocatePCB();

pcb* setupPCB(char *name, int classCode, int priorityCode);

#endif
