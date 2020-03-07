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
			freePCB(cop);
		}
		if (getOpCode() == IDLE){
			cop->top = (unsigned char*)registers;
			insertPCB(cop);
		}
	}
	if (currentPcb != NULL){
		cop = currentPcb;
		removePCB(currentPcb);
		return (u32int*) cop->top;
	}
	else{
		return (u32int*) currentContext;
	}
}
