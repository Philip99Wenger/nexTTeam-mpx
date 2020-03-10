#include "pcb.h"

Queue readyQueue;
Queue blockedQueue;
Queue suspendReadyQueue;
Queue suspendedBlockedQueue;

void insertPCB(pcb* Pcb){
	if(Pcb->stateRRB == READY && Pcb->stateIsSuspended == NOTSUSPENDED){
		if(readyQueue.head==NULL){
			readyQueue.head = Pcb;
			readyQueue.tail = readyQueue.head;
			Pcb->pcbPrev = NULL;
			Pcb->pcbNext = NULL;
			readyQueue.count++;
			return;
		}
		pcb* current = readyQueue.head;
		pcb* previous;
		if(Pcb->priority < current->priority){ //if head priority value is higher than pcb priority, place at the head
				readyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				readyQueue.count++;
				return;
			}
		previous = readyQueue.head;
		while(current){ // place priority where needed in queue
			if(Pcb->priority < current->priority){
				previous = current->pcbPrev;
				previous->pcbNext = Pcb;
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
		if(suspendReadyQueue.head==NULL){
			suspendReadyQueue.head = Pcb;
			suspendReadyQueue.tail = suspendReadyQueue.head;
			Pcb->pcbPrev = NULL;
			Pcb->pcbNext = NULL;
			readyQueue.count++;
			return;
		}
		pcb* current = suspendReadyQueue.head;
		pcb* previous;
		if(Pcb->priority < current->priority){
				suspendReadyQueue.head = Pcb;
				current->pcbPrev = Pcb;
				Pcb->pcbNext = current;
				suspendReadyQueue.count++;
				return;
			}
		previous=suspendReadyQueue.head;
		while(current){ // place priority where needed in queue
			if(Pcb->priority < current->priority){
				previous = current->pcbPrev;
				previous->pcbNext = Pcb;
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
	
	//Testing if name came in okay (results were as expected)
	//int nameSize = strlen(PcbName);
	//sys_req(WRITE, DEFAULT_DEVICE, PcbName, &nameSize);
			
	if(findPCB(PcbName) != NULL){
		char repeat[] = "There already exists a process with this name. Cannot create another one.";
		int repeatSize = strlen(repeat);
		sys_req(WRITE, DEFAULT_DEVICE, repeat, &repeatSize);
	}else{
		newPCB->namePtr = newPCB->processName;	
		newPCB->namePtr = strcpy(newPCB->namePtr, PcbName);
		newPCB->priority = priorityCode;
		newPCB->stateRRB = 0;			//Ready(0)
		newPCB->stateIsSuspended = 0;		//Not-Suspended(0)
		newPCB->classIsApp = classCode;		//Application(1)/System-Process(0)
	
		newPCB->base = &(newPCB->stack[0]);
		newPCB->top = &(newPCB->stack[1024-sizeof(context)]);
		int i;
		for(i=0; i<(newPCB->top-newPCB->base); i++){
			newPCB->stack[i]=NULL;
		}

		//Testing if name came in okay (results were as expected)
		//int nameSize = strlen(newPCB -> namePtr);
		//sys_req(WRITE, DEFAULT_DEVICE, newPCB -> namePtr, &nameSize);

		return newPCB;
	}
	return NULL;
	
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

int removePCB(pcb* process) {
	int success = 0;
	int error = -1;
	
	//check for infinite
	char heyStop[100] = "\x1B[31mHey, Stop! That's supposed to be infinite. Don't delete.\x1B[37m\n";
	int stopSize = strlen(heyStop);
	char infiniteName[] = "infinite";
	pcb* isItInfinite = findPCB(infiniteName);
	if((isItInfinite!=NULL)&&(process==isItInfinite)&&(process->stateIsSuspended==0)){
		sys_req(WRITE, DEFAULT_DEVICE, heyStop, &stopSize);		
		return error;
	}


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
		return error;//This PCB already doesn't exist
	} else if(process->pcbPrev == NULL){//Are we at an end of the list?
		pcb* next = process->pcbNext;
		next->pcbPrev = NULL;
		return success;//Success!
	} else if(process->pcbNext == NULL){//Are we at an end o the list?
		pcb* previous = process->pcbPrev;
		previous->pcbNext = NULL;
		return success;//Success!
	} else{//We must be in the middle somewhere
		pcb* previous = process->pcbPrev;
		pcb* next = process->pcbNext;
		previous->pcbNext = next;
		next->pcbPrev = previous;
		return success;//Success!
	}
	
	//If it worked we wouldn't still be here.	
	return error;// error :/
}

void setPriority(pcb* priorityPCB, int priorityNum){
	char noPCB[] = "Priority has successfully been changed.\n";
	int noSize = strlen(noPCB);
		priorityPCB->priority = priorityNum;
		removePCB(priorityPCB);
		insertPCB(priorityPCB);
		sys_req(WRITE, DEFAULT_DEVICE, noPCB, &noSize);
}

void showReady(){
	char ready[] = "\nReady Queue:\n";
	int readySize = strlen(ready);
	char suspendReady[] = "\nSuspend Ready Queue:\n";
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
	char blocked[] = "\nBlocked Queue:\n";
	int blockedSize = strlen(blocked);
	char suspendBlocked[] = "\nSuspend Blocked Queue:\n";
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

void showAllProcesses(){
	showReady();
	showBlocked();
}

void showPCB(char *name){
	char noPCB[] = "There is no PCB with that name.\n";
	int noSize = strlen(noPCB);
	pcb* onePCB = findPCB(name);
	if (onePCB != NULL){
		printOnePCB(onePCB);
	}
	else{
		sys_req(WRITE, DEFAULT_DEVICE, noPCB, &noSize);
	}
}

void printOnePCB(pcb* Pcb){
	//Name strings
	char name[] = "\nName: ";
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
	char newLine[] = "\n";
	int newlineSize = strlen(newLine);	

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
	sys_req(WRITE, DEFAULT_DEVICE, newLine, &newlineSize);
}
void block(pcb* PCB){
	//finding and checking pcb validity in wrapper		
	removePCB(PCB);	//Removes PCB from current queue
	PCB->stateRRB = 2;
	insertPCB(PCB);	//Inserts PCB into appropriate queue (ie blocked queue)

	char success[] = "PCB has been successfully blocked\n";
	int successSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);

}
void unblock(pcb* PCB){
	//finding and checking pcb validty in wrapper
	removePCB(PCB);	//Remove PCB from blocked queue
	PCB->stateRRB = 0;
	insertPCB(PCB);	//Inserts PCB into appropriate queue (ie ready queue)

	char success[] = "PCB has been successfully unblocked\n";
	int successSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);


}

int freePCB(pcb* PCB){
	sys_free_mem((PCB->processName));		//The name of the process
	sys_free_mem((PCB->namePtr));	//Pointer to the name
	sys_free_mem(&(PCB->priority));			//0-9 ranking of importance, with 9 going first
	sys_free_mem(&(PCB->stateRRB));			//Ready(0)/Running(1)/Blocked(2)
	sys_free_mem(&(PCB->stateIsSuspended));		//Suspended(1)/Not-Suspended(0)
	sys_free_mem(&(PCB->classIsApp));			//Application(1)/System-Process(0)
	sys_free_mem((PCB-> pcbNext));		//Pointer to the start of the next process
	sys_free_mem((PCB-> pcbPrev));		//Pointer to the start of the last process
		
	sys_free_mem((PCB-> base));		//Pointer to the base of the process
	sys_free_mem((PCB-> top));

	sys_free_mem(PCB);

	return 0;

}

void suspend(pcb* PCB){
	//finding and checking pcb validity in wrapper		
	removePCB(PCB);	//Removes PCB from current queue
	PCB->stateIsSuspended = 1;
	insertPCB(PCB);	//Inserts PCB into appropriate queue 

	char success[] = "PCB has been successfully suspended\n";
	int successSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);

}
void resume(pcb* PCB){
	//finding and checking pcb validty in wrapper
	
	removePCB(PCB);	//Remove PCB from blocked queue
	/*char repeat[] = "pos alpha.2.\n";
	int repeatSize = strlen(repeat);
	sys_req(WRITE, DEFAULT_DEVICE, repeat, &repeatSize);*/
	PCB->stateIsSuspended = 0;
	insertPCB(PCB);	//Inserts PCB into appropriate queue 

	char success[] = "PCB has been successfully resumed\n";
	int successSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);


}

pcb* getReadyQueueHead(){
	return readyQueue.head;
}

pcb* nextProcess(){
	pcb* next = readyQueue.head;
	while (next != NULL){
		next = next->pcbNext;
	}

	return next;
}



