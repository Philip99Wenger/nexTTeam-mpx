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

int isEmpty(){
	int present = 1;
	if(allocatedBlocks.head!=NULL){present = 0;}
	return present;
}

