#include "pcb.h"

Queue readyQueue;
Queue blockedQueue;
Queue suspendReadyQueue;
Queue suspendedBlockedQueue;

void insertPCB(pcb* Pcb){
	if(Pcb->stateRRB == READY && Pcb->stateIsSuspended == NOTSUSPENDED){
		pcb* current = readyQueue.head;
		pcb* previous;
		if(Pcb->priority < current->priority){ //if head priority value is higher than pcb priority, place at the head
				readyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
		while(current){ // place priority where needed in queue
			if(Pcb->priority < current->priority){
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
		if(Pcb->priority < current->priority){
				suspendReadyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				suspendReadyQueue.count++;
				return;
			}
		while(current){ // place priority where needed in queue
			if(Pcb->priority < current->priority){
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

void removePCB(pcb* process) {
	//char* success[] = "success";
	//char* error[] = "error";
	//Queue thisQueue = NULL;
	
	//Which Queue?
	if(process->stateRRB == 0 && process->stateIsSuspended == 0){
		if(readyQueue.head == process){//Is this a queue's head?
			readyQueue.head = process->pcbNext;//Chop that off
		} else if(readyQueue.tail == process){//Is this a queue's tail?
			readyQueue.tail = process->pcbPrev;//snip snip snip
		}
		readyQueue.count--;
	} else if(process->stateRRB == 2 && process->stateIsSuspended == 0){
		if(blockedQueue.head == process){//Is this a queue's head?
			blockedQueue.head = process->pcbNext;//Chop that off
		} else if(blockedQueue.tail == process){//Is this a queue's tail?
			blockedQueue.tail = process->pcbPrev;//snip snip snip
		}
		blockedQueue.count--;
	} else if(process->stateRRB == 0 && process->stateIsSuspended == 1){
		if(suspendReadyQueue.head == process){//Is this a queue's head?
			suspendReadyQueue.head = process->pcbNext;//Chop that off
		} else if(suspendReadyQueue.tail == process){//Is this a queue's tail?
			suspendReadyQueue.tail = process->pcbPrev;//snip snip snip
		}
		suspendReadyQueue.count--;
	} else if(process->stateRRB == 2 && process->stateIsSuspended == 1){
		if(suspendedBlockedQueue.head == process){//Is this a queue's head?
			suspendedBlockedQueue.head = process->pcbNext;//Chop that off
		} else if(suspendedBlockedQueue.tail == process){//Is this a queue's tail?
			suspendedBlockedQueue.tail = process->pcbPrev;//snip snip snip
		}
		suspendedBlockedQueue.count--;
	}

	//Remove from Doubly-Linked List
	if((process->pcbPrev == NULL) && (process->pcbNext == NULL)){
		;//Do Nothing because this doesn't exist anyway
	} else if(process->pcbPrev == NULL){//Are we at an end of the list?
		pcb* next = process->pcbNext;
		next->pcbPrev = NULL;
	} else if(process->pcbNext == NULL){//Are we at an end o the list?
		pcb* previous = process->pcbPrev;
		previous->pcbNext = NULL;
	} else{//We must be in the middle somewhere
		pcb* previous = process->pcbPrev;
		pcb* next = process->pcbNext;
		previous->pcbNext = next;
		next->pcbPrev = previous;
	}
	
	//Complete	
	return;// success;
}
