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

	//UNCOMMENT BELOW ONCE INSERT IS CREATED
	//insertMCB(head);
	freeBlocks.head = head;
	allocatedBlocks.head = NULL;

	return actualSize;
}

void freeMem(char* toFree){
	if(allocatedBlocks.head == NULL){	//ERROR CHECK: If allocated list is empty
		char error1[] = "\nCannot free memory. No allocated memory to free.\n";
		int error1Size = strlen(error1);
		sys_req(WRITE, DEFAULT_DEVICE, error1, &error1Size);
	}else{
		MCB* current = allocatedBlocks.head;
		while (current != NULL){	//ITERATE THROUGH ALLOCATED LIST
			if(current->pcbName == toFree){
				//Logic to free block
				
				//TODO unlink AMCB
				
				
				//TODO create FMCB at start of block
				
				
				//TODO modify MCB
				current->type = 1;	//Changes type to free
								
				//TODO link into free list in order by address


				
				//SUCCESS MESSAGE	
				char success[] = "Memory Block has been freed.";
				int successSize = strlen(success);
				sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);
				
				//TODO if new linked free block is adjacent to another free block, merge into one

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
	char size[] = "\nSize: ";
	int sizeSize = strlen(size);
	char mcbSize[10];

	//Print size
	sys_req(WRITE, DEFAULT_DEVICE, size, &sizeSize);
	mcbSize[9] = '\0';
	strcpy(mcbSize, intToAscii(currentMCB->size));
	int mcbSizeSize = strlen(mcbSize);
	sys_req(WRITE, DEFAULT_DEVICE, mcbSize, &mcbSizeSize);
}

int checkIfEmpty(){
	int notPresent = 1;					//1 if empty
	if(startHeap==NULL){notPresent = 2;} else{		//2 if there is no heap
		if(allocatedBlocks.head!=NULL){notPresent = 0;}	//0 if NOT empty
	}
	return notPresent;
}

