#include "context.h"

pcb *cop;
context *currentContext;

u32int* sys_call(context *registers){
	char version[] = "\x1B[36mVERSION: 3.0\x1B[37m\n";
	int versionSize = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
	pcb *currentPcb;
	currentPcb = getReadyQueueHead();
	if (cop == NULL){
		currentContext = registers;
	}
	else {
		if (getOpCode() == EXIT){
			cop -> stateRRB = 0;	//Ready state
			//testing one line below
			removePCB(cop);	
			freePCB(cop);
		}
		if (getOpCode() == IDLE){
			cop->top = (unsigned char*)registers;
			//testing one line below
			removePCB(cop);
			insertPCB(cop);
		}
	}

	cop = nextProcess();	

	if (currentPcb != NULL){
		currentPcb -> stateRRB = 1;	//Running state
		cop = currentPcb;
		removePCB(currentPcb);
		return (u32int*) cop->top;
	}
	else{
		return (u32int*) currentContext;
	}
}
