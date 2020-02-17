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
		// if pcb is the lowest priority
		current = readyQueue.tail;
		Pcb->pcbPrev = current;
		Pcb->pcbNext = NULL;
		current->pcbNext = Pcb;
		readyQueue.tail = Pcb;
		readyQueue.count++;
		return;
	}
	else if(Pcb->stateRRB == READY && Pcb->stateIsSuspended == SUSPENDED){
		pcb* current = suspendReadyQueue.head;
		pcb* previous;
		if(Pcb->priority > current->priority){
				suspendReadyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				suspendReadyQueue.count++;
				return;
			}
		while(current){ // place priority where needed in queue
			if(Pcb->priority > current->priority){
				previous->pcbNext = current;
				Pcb->pcbPrev = previous;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				suspendReadyQueue.count++;
				return;
			}
			previous = current;
			current = current->pcbNext;
		}
		// if pcb is the lowest priority
		current = suspendReadyQueue.tail;
		Pcb->pcbPrev = current;
		Pcb->pcbNext = NULL;
		current->pcbNext = Pcb;
		suspendReadyQueue.tail = Pcb;
		suspendReadyQueue.count++;
		return;
	}

	else if(Pcb->stateRRB == BLOCKED && Pcb->stateIsSuspended == NOTSUSPENDED){
		if(blockedQueue.count == 0){
				Pcb->pcbPrev = NULL;
				Pcb->pcbNext = NULL;
				blockedQueue.head = Pcb;
				blockedQueue.tail = Pcb;
				blockedQueue.count++;
				return;
			}
		else{
				blockedQueue.tail->pcbNext = Pcb;
				Pcb->pcbNext = NULL;
				Pcb->pcbPrev = blockedQueue.tail;
				blockedQueue.tail = Pcb;
				blockedQueue.count++;
				return;
				
			}
	}

	else if(Pcb->stateRRB == BLOCKED && Pcb->stateIsSuspended == SUSPENDED){
		if(suspendedBlockedQueue.count == 0){
				Pcb->pcbPrev = NULL;
				Pcb->pcbNext = NULL;
				suspendedBlockedQueue.head = Pcb;
				suspendedBlockedQueue.tail = Pcb;
				suspendedBlockedQueue.count++;
				return;
			}
		else{
				suspendedBlockedQueue.tail->pcbNext = Pcb;
				Pcb->pcbNext = NULL;
				Pcb->pcbPrev = blockedQueue.tail;
				suspendedBlockedQueue.tail = Pcb;
				suspendedBlockedQueue.count++;
				return;
				
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
