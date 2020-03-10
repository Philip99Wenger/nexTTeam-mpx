#include "context.h"

pcb *cop;
context *currentContext;

u32int* sys_call(context *registers){
	pcb *currentPcb;
	currentPcb = getReadyQueueHead();
	if (cop == NULL){
		currentContext = registers;
	}
	else {
		if (getOpCode() == EXIT){
			cop -> stateRRB = 0;	//Ready state
			freePCB(cop);
		}
		if (getOpCode() == IDLE){
			cop->top = (unsigned char*)registers;
			insertPCB(cop);
		}
	}
	if (currentPcb != NULL){
		currentPcb -> stateRRB = 1;	//Running state
		cop = currentPcb;
		removePCB(currentPcb);
		outb(0x20, 0x20);
		return (u32int*) cop->top;
	}
	else{
		return (u32int*) currentContext;
	}
}
