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

