#include "PCB.h"

Queue readyQueue;
Queue blockedQueue;
Queue suspendReadyQueue;
Queue suspendedBlockedQueue;

void insertPCB(pcb* Pcb){
	if(Pcb->stateRRB == 0 && Pcb->stateIsSuspended == 0){
		pcb* current = readyQueue.head;
		if(Pcb->priority > current->priority){
			readyQueue.head = pcb;
			pcb->pcbNext = current;
			current->pcbPrev = pcb;
			readyQueue.count++;
		}
	}
}
