#include "mcb.h"

ucstar startHeap;
ucstar endHeap;
memoryList freeBlocks;
memoryList allocatedBlocks;

int initializeHeap(int size){
	int actualSize = size + sizeof(MCB);

	startHeap = kmalloc(actualSize);
	if(!startHeap) {
		return -1;
	}
	
	MCB* head = (MCB*) startHeap;
	head->type = FREE;
	head->startAddress = getAddress(head);
	head->size = actualSize;
	head->pcbName = "Starting MCB Free Block";
	head->previous = NULL;
	head->next = NULL;
	endHeap = startHeap + actualSize;

	freeBlocks.head = head;
	allocatedBlocks.head = NULL;

	return actualSize;
}

void *allocateMem(int size){
	int actualSize = size+ sizeof(MCB);
	MCB* curMCB= freeBlocks.head;
	while(curMCB->size<actualSize){
		curMCB=curMCB->next;
		if(curMCB==NULL){
			return NULL;
		}
	}
	//unlink the MCB from the free list
	removeMCB(curMCB);
	MCB* allocMCB=NULL; //unsure how to intialize this properly
	allocMCB->type = ALLOCATED;
	allocMCB->startAddress=curMCB->startAddress;
	allocMCB->size=actualSize;
	allocMCB->pcbName= "new allocated block"; //uncertain if this is meant to be a parameter or what

	MCB* freeMCB=NULL; //unsure how to intialize this properly
	freeMCB->type = FREE;
	allocMCB->startAddress=curMCB->startAddress+actualSize;
	allocMCB->size=curMCB->size-actualSize;
	allocMCB->pcbName= "new free block"; //uncertain if this is meant to be a parameter or what

	
	//put the mcb at the front of the memory blocks ???????????????
	//allocMCB->startAddress-sizeof(MCB)=allocMCB;
	//insertMCB(allocMCB);
	//freeMCB->startAddress-sizeof(MCB)=freeMCB;
	//insertMCB(freeMCB);
	char address[] = "\nAddress offset: ";
	int addressSize = strlen(address);
	char newline[] = " \n";
	int newlineSize = strlen(newline);
	char mcbAddress[10];

	//Print size
	sys_req(WRITE, DEFAULT_DEVICE, address, &addressSize);
	mcbAddress[9] = '\0';
	strcpy(mcbAddress, intToAscii((allocMCB->startAddress)-startHeap));
	int mcbAddressSize = strlen(mcbAddress);
	sys_req(WRITE, DEFAULT_DEVICE, mcbAddress, &mcbAddressSize);
	sys_req(WRITE, DEFAULT_DEVICE, newline, &newlineSize);

	


	sortedInsert(&allocatedBlocks,allocMCB); //insert the new block into the allocatedBlocks
	sortedInsert(&freeBlocks,freeMCB); //insert the free block into the freeBlocks
	

	return (void*)allocMCB->startAddress;
}

void sortedInsert(memoryList* curList,MCB* newBlock){
	if (curList->head==NULL){
		curList->head=newBlock;
	}
	
	else if ( curList->head->startAddress >= newBlock->startAddress){
		newBlock->next = curList->head;
		newBlock->next->previous=newBlock;
		curList->head= newBlock;
	}
	else{
		MCB* cur = curList->head;
		while ( cur->next != NULL && cur->next->startAddress < newBlock->startAddress){
			cur=cur->next;
		}
		newBlock->next=cur->next;
		if(cur->next != NULL){
			newBlock->next->previous=newBlock;
		}
		cur->next=newBlock;
		newBlock->previous=cur;
	}

}



void freeMem(ucstar toFreeAddress){	
	if(allocatedBlocks.head == NULL){	//ERROR CHECK: If allocated list is empty
		char error1[] = "\nCannot free memory. No allocated memory to free.\n";
		int error1Size = strlen(error1);
		sys_req(WRITE, DEFAULT_DEVICE, error1, &error1Size);
	}else{
		
		//Search for MCB that matches address
		MCB* current = allocatedBlocks.head;
		while (current != NULL){
			if(current->startAddress == toFreeAddress){
				//Logic to free block
				
				//unlink from allocated list
				removeMCB(current);
				
				//modify MCB
				current->type = 1;	//Changes type to free
								
				//link into free list in order by address
				sortedInsert(&freeBlocks, current);

				
				//SUCCESS MESSAGE	
				char success[] = "Memory Block has been freed.";
				int successSize = strlen(success);
				sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);
				
				//if new linked free block is adjacent to another free block, merge into one

				if(current->next != NULL && getAddress(current->next) == (getAddress(current)+current->size)){	//checks if next is adjacent
					current->size = current->size + current->next->size;
					removeMCB(current->next);
				}else if(current->previous != NULL && getAddress(current->previous) == (getAddress(current)+current->size)){
					current->size = current->size + current->previous->size;
					current->startAddress = current->previous->startAddress;
					removeMCB(current->previous);
				}

				//BREAK OUT OF SEARCH LOOP
				break;
			}else{
				current = current->next;
			}
		}
	}
}

ucstar getAddress (MCB* mcb) {
	int i;
	ucstar address;
	
	address = (ucstar) mcb;
	for(i=0; i<(int)sizeof(MCB); i++) {
		address++;
	}
	
	return (ucstar) address;
}

void showAllocated(){
	char allocated[] = "\nAllocated Memory:\n";
	int allocatedSize = strlen(allocated);

	//Print blocked queue
	sys_req(WRITE, DEFAULT_DEVICE, allocated, &allocatedSize);
	MCB* current = allocatedBlocks.head;
	while (current != NULL){
		printOneMCB(current);
		current = current->next;
	}
}

void showFree(){
	char free[] = "\nFree Memory:\n";
	int freeSize = strlen(free);

	//Print blocked queue
	sys_req(WRITE, DEFAULT_DEVICE, free, &freeSize);
	MCB* current = freeBlocks.head;
	while (current != NULL){
		printOneMCB(current);
		current = current->next;
	}

}

void printOneMCB(MCB* currentMCB){
	//Size strings
	char address[] = "\nAddress: ";
	int addressSize = strlen(address);
	char size[] = "\nSize: ";
	int sizeSize = strlen(size);
	char mcbAddress[10];
	char mcbSize[10];

	//Print size
	sys_req(WRITE, DEFAULT_DEVICE, address, &addressSize);
	mcbAddress[9] = '\0';
	strcpy(mcbAddress, intToAscii(currentMCB->startAddress));
	int mcbAddressSize = strlen(mcbAddress);
	sys_req(WRITE, DEFAULT_DEVICE, mcbAddress, &mcbAddressSize);
	sys_req(WRITE, DEFAULT_DEVICE, size, &sizeSize);
	mcbSize[9] = '\0';
	strcpy(mcbSize, intToAscii(currentMCB->size));
	int mcbSizeSize = strlen(mcbSize);
	sys_req(WRITE, DEFAULT_DEVICE, mcbSize, &mcbSizeSize);
	char newLine[] = "\n";
	int newLineSize = strlen(newLine);
	sys_req(WRITE, DEFAULT_DEVICE, newLine, &newLineSize);
}

int checkIfEmpty(){
	int notPresent = 1;					//1 if empty
	if(startHeap==NULL){notPresent = 2;} else{		//2 if there is no heap
		if(allocatedBlocks.head!=NULL){notPresent = 0;}	//0 if NOT empty
	}
	return notPresent;
}

void removeMCB(MCB* mcb){
	if(mcb == freeBlocks.head){
		freeBlocks.head = NULL;
		if(mcb->next != NULL){
			freeBlocks.head = mcb->next;
			mcb->next->previous = NULL;
		}
	}else if (mcb == allocatedBlocks.head){
		allocatedBlocks.head = NULL;
		if(mcb->next != NULL){
			allocatedBlocks.head = mcb->next;
			mcb->next->previous = NULL;
		}
	}else if (mcb->next == NULL){
		mcb->previous->next = NULL;
	}else{
		mcb->previous->next = mcb->next;
		mcb->next->previous = mcb->previous;
	}
}

