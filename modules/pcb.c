#include "pcb.h"

Queue readyQueue;
Queue blockedQueue;
Queue suspendReadyQueue;
Queue suspendedBlockedQueue;

void insertPCB(pcb* Pcb){
	if(Pcb->stateRRB == READY && Pcb->stateIsSuspended == NOTSUSPENDED){
		pcb* current = readyQueue.head;
		pcb* previous;
		if(Pcb->priority > current->priority){
				readyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
		while(current){ // place priority where needed in queue
			if(Pcb->priority > current->priority){
				previous->pcbNext = current;
				Pcb->pcbPrev = previous;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
			previous = current;
			current = current->pcbNext;
		}
		
	}
}

pcb* allocatePCB(){
	pcb * thisPCB = (pcb *) sys_alloc_mem(sizeof(pcb));
	
	return thisPCB;
}

pcb* setupPCB(char *PcbName, int classCode, int priorityCode){
	pcb * newPCB = allocatePCB();
	strcpy(newPCB->namePtr, PcbName);
	newPCB->priority = priorityCode;
	newPCB->stateRRB = 0;			//Ready(0)
	newPCB->stateIsSuspended = 0;		//Not-Suspended(0)
	newPCB->classIsApp = classCode;		//Application(1)/System-Process(0)

	return newPCB;
	
}
