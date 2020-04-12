#ifndef MCB_H_

#define MCB_H_

#define ALLOCATED 0
#define FREE 1

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
#include "context.h"
#include "pcb.h"

typedef unsigned long ucstar;

typedef struct MCB {
	int type;
	ucstar startAddress;
	int size;
	char* pcbName;
	struct MCB* previous;
	struct MCB* next;
	
}MCB;


typedef struct memoryList {
	MCB* head;
} memoryList;

int initializeHeap(int size);
<<<<<<< HEAD
int freeMem(void *toFree);
void *allocateMem(int size);
=======
void freeMem(ucstar toFreeAddress);
u32int allocateMem(u32int size);
>>>>>>> adb88c33dd51e968315f17d8f2fba67e6a230e3d
void sortedInsert(memoryList* curList,MCB* newBlock);
ucstar getAddress (MCB* mcb);
void showAllocated();
void showFree();
void printOneMCB(MCB* currentMCB);
//! Checks whether the heap is empty & free or not.
int checkIfEmpty();
void removeMCB(MCB* mcb);

#endif

