#include "PCB.h"

Queue readyQueue;
Queue blockedQueue;
Queue suspendReadyQueue;
Queue suspendedBlockedQueue;

void insertPCB(pcb* Pcb){
	if(Pcb->stateRRB == READY && Pcb->stateIsSuspended == NOTSUSPENDED){
		pcb* current = readyQueue.head;
		pcb* previous;
		if(Pcb->priority > current->priority){
				readyQueue.head = pcb;
				current->pcbPrev = pcb;
				pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
		while(current){ // place priority where needed in queue
			if(Pcb->priority > current->priority){
				previous->pcbNext = current;
				pcb->pcbPrev = previous;
				current->pcbPrev = pcb;
				pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
			previous = current;
			current = current->next
		}
		
	}
}

pcb* allocatePCB(){
	pcb * thisPCB = (pcb *) sys_alloc_mem(sizeof(pcb));
	
	return thisPCB;
}

pcb* pcb* setupPCB(char *PcbName, int classCode, int priorityCode){
	pcb * newPCB = allocatePCB();
	strcpy(newPCB->name, Pcbname);
	newPCB->priority = priorityCode;
	stateRRB = 0;			//Ready(0)
	stateIsSuspended = 0;		//Not-Suspended(0)
	classIsApp = classCode;		//Application(1)/System-Process(0)

	return newPCB;
	
}
