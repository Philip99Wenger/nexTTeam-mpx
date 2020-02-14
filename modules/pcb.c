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
