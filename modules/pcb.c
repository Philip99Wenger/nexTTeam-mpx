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

pcb* findPCB(char *PcbName){
	pcb* current = readyQueue.head;
	while(current){ // loops through looking for name
		if(strcmp(PcbName , current->namePtr)==0){
			return current;
		}
		current = current->pcbNext;
	}
	current = blockedQueue.head;
	while(current){ // loops through looking for name
		if(strcmp(PcbName , current->namePtr)==0){
			return current;
		}
		current = current->pcbNext;
	}
	current = suspendReadyQueue.head;
	while(current){ // loops through looking for name
		if(strcmp(PcbName , current->namePtr)==0){
			return current;
		}
		current = current->pcbNext;
	}
	current = suspendedBlockedQueue.head;
	while(current){ // loops through looking for name
		if(strcmp(PcbName , current->namePtr)==0){
			return current;
		}
		current = current->pcbNext;
	}
	return NULL;
	
	
	
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

void showReady(){
	char ready[] = "Ready Queue:";
	int readySize = strlen(ready);
	char suspendReady[] = "Suspend Ready Queue:";
	int suspendReadySize = strlen(suspendReady);

  	//Print ready queue
	sys_req(WRITE, DEFAULT_DEVICE, ready, &readySize);
	pcb* current = readyQueue.head;
	while (current != NULL){
		printOnePCB(current);
		current = current->pcbNext;
	}

	//Print suspend ready queue
	sys_req(WRITE, DEFAULT_DEVICE, suspendReady, &suspendReadySize);
	current = suspendReadyQueue.head;
	while (current != NULL){
		printOnePCB(current);
		current = current->pcbNext;
	}
}

void showBlocked(){
	char blocked[] = "Blocked Queue:";
	int blockedSize = strlen(blocked);
	char suspendBlocked[] = "Suspend Blocked Queue:";
	int suspendBlockedSize = strlen(suspendBlocked);

	//Print blocked queue
	sys_req(WRITE, DEFAULT_DEVICE, blocked, &blockedSize);
	pcb* current = blockedQueue.head;
	while (current != NULL){
		printOnePCB(current);
		current = current->pcbNext;
	}

	//Print suspend ready queue
	sys_req(WRITE, DEFAULT_DEVICE, suspendBlocked, &suspendBlockedSize);
	current = suspendedBlockedQueue.head;
	while (current != NULL){
		printOnePCB(current);
		current = current->pcbNext;
	}
}

void showAll(){
	showReady();
	showBlocked();
}

void printOnePCB(pcb* Pcb){
	//Name strings
	char name[] = "\n\nName: ";
	int nameSize = strlen(name);

	//class strings
	char class[] = "\nClass: ";
	int classSize = strlen(class);
	char application[] = "Application";
	int appSize = strlen(application);
	char system[] = "System-Process";
	int systemSize = strlen(system);

	//state strings
	char state[] = "\nState: ";
	int stateSize = strlen(state);
	char blocked[] = "Blocked";
	int blockedSize = strlen(blocked);
	char running[] = "Running";
	int runSize = strlen(running);
	char ready[] = "Ready";
	int readySize = strlen(ready);

	//suspend state strings
	char suspend[] = "\nSuspend State: ";
	int suspendSize = strlen(suspend);
	char suspended[] = "Suspended";
	int susSize = strlen(suspended);
	char notSus[] = "Not Suspended";
	int notSize = strlen(notSus);

	//priority strings
	char priorityNum[] = "\nPriority: ";
	int prioritySize = strlen(priorityNum);
	char currentPriority[5];
	int currPriSize;
	
	//Print the attributes for one PCB
	//Print the name
	sys_req(WRITE, DEFAULT_DEVICE, name, &nameSize);
	int namePtrSize = strlen(Pcb->namePtr);
	sys_req(WRITE, DEFAULT_DEVICE, Pcb->namePtr, &namePtrSize);

	//Print the class
	sys_req(WRITE, DEFAULT_DEVICE, class, &classSize);
	if(Pcb->classIsApp == 1){
		sys_req(WRITE, DEFAULT_DEVICE, application, &appSize);
	}
	else if(Pcb->classIsApp == 0){
		sys_req(WRITE, DEFAULT_DEVICE, system, &systemSize);
	}

	//Print the state
	sys_req(WRITE, DEFAULT_DEVICE, state, &stateSize);
	if(Pcb->stateRRB == 2){
		sys_req(WRITE, DEFAULT_DEVICE, blocked, &blockedSize);
	}
	else if(Pcb->stateRRB == 1){
		sys_req(WRITE, DEFAULT_DEVICE, running, &runSize);
	}
	else if(Pcb->stateRRB == 0){
		sys_req(WRITE, DEFAULT_DEVICE, ready, &readySize);
	}

	//Print the suspend state
	sys_req(WRITE, DEFAULT_DEVICE, suspend, &suspendSize);
	if(Pcb->stateIsSuspended == 1){
		sys_req(WRITE, DEFAULT_DEVICE, suspended, &susSize);
	}
	else if(Pcb->stateIsSuspended == 0){
		sys_req(WRITE, DEFAULT_DEVICE, notSus, &notSize);
	}

	//Print the priority
	sys_req(WRITE, DEFAULT_DEVICE, priorityNum, &prioritySize);
	strcpy(currentPriority, intToAscii(Pcb->priority));
	currPriSize = strlen(currentPriority);
	sys_req(WRITE, DEFAULT_DEVICE, currentPriority, &currPriSize);
}
