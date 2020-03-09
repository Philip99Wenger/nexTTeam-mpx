#include "r3commands.h"


void yield(){

	asm volatile("int $60");

}


void loadr3(){

	//DEFINE STACK_SIZE!!!!!!!HOW BIG DOES THIS NEED TO BE?
	unsigned int stack_size = 2;

	//create new pcb for PROCESS 1

	char * proc1Name = "Proc1";

	pcb * proc1pcb = setupPCB(proc1Name, 1, 9);	//char *PcbName, int classCode, int priorityCode

	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!

	proc1pcb->base = sys_alloc_mem(stack_size);

	proc1pcb->top = (proc1pcb->base) + 1024 - sizeof(struct context);

	suspend(proc1pcb);	//set to suspended ready

	context * cp = (context*)(proc1pcb -> stack_top);

	memeset(cp, 0, sizeof(context));

	cp -> fs = 0 x10;

	cp -> gs = 0 x10;

	cp -> ds = 0 x10;

	cp -> es = 0 x10;

	cp -> cs = 0 x8;

	cp -> ebp (u32int)(proc1pcb -> stack);

	cp -> esp (u32int)(proc1pcb -> stack_top);

	cp -> eip (u32int)proc1;//The function correlating to the process, ie. Proc1

	cp -> eflags


	//create new pcb for PROCESS 2

	char * proc2Name = "Proc2";

	pcb * proc2pcb = setupPCB(proc2Name, 1, 8);

	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!

	proc2pcb->base = sys_alloc_mem(stack_size);

	proc2pcb->top = (proc2pcb->base) + 1024 - sizeof(struct context);

	context * cp = (context*)(proc2pcb -> stack_top);

	memeset(cp, 0, sizeof(context));

	cp -> fs = 0 x10;

	cp -> gs = 0 x10;

	cp -> ds = 0 x10;

	cp -> es = 0 x10;

	cp -> cs = 0 x8;

	cp -> ebp (u32int)(proc2pcb -> stack);

	cp -> esp (u32int)(proc2pcb -> stack_top);

	cp -> eip (u32int)proc2;

	cp -> eflags


	//create new pcb for PROCESS 3

	char * proc3Name = "Proc3";

	pcb * proc3pcb = setupPCB(proc3Name, 1, 7);

	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!

	proc3pcb->base = sys_alloc_mem(stack_size);

	proc3pcb->top = (proc3pcb->base) + 1024 - sizeof(struct context);

	context * cp = (context*)(proc3pcb -> stack_top);

	memeset(cp, 0, sizeof(context));

	cp -> fs = 0 x10;

	cp -> gs = 0 x10;

	cp -> ds = 0 x10;

	cp -> es = 0 x10;

	cp -> cs = 0 x8;

	cp -> ebp (u32int)(proc3pcb -> stack);

	cp -> esp (u32int)(proc3pcb -> stack_top);

	cp -> eip (u32int)proc3;

	cp -> eflags


	//create new pcb for PROCESS 4

	char * proc4Name = "Proc4";

	pcb * proc4pcb = setupPCB(proc4Name, 1, 6);

	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!

	proc4pcb->base = sys_alloc_mem(stack_size);

	proc4pcb->top = (proc4pcb->base) + 1024 - sizeof(struct context);

	context * cp = (context*)(proc4pcb -> stack_top);

	memeset(cp, 0, sizeof(context));

	cp -> fs = 0 x10;

	cp -> gs = 0 x10;

	cp -> ds = 0 x10;

	cp -> es = 0 x10;

	cp -> cs = 0 x8;

	cp -> ebp (u32int)(proc4pcb -> stack);

	cp -> esp (u32int)(proc4pcb -> stack_top);

	cp -> eip (u32int)proc4;

	cp -> eflags


	//create new pcb for PROCESS 5

	char * proc5Name = "Proc5";

	pcb * proc5pcb = setupPCB(proc5Name, 1, 5);

	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!

	proc5pcb->base = sys_alloc_mem(stack_size);

	proc5pcb->top = (proc5pcb->base) + 1024 - sizeof(struct context);

	context * cp = (context*)(proc5pcb -> stack_top);

	memeset(cp, 0, sizeof(context));

	cp -> fs = 0 x10;

	cp -> gs = 0 x10;

	cp -> ds = 0 x10;

	cp -> es = 0 x10;

	cp -> cs = 0 x8;

	cp -> ebp (u32int)(proc5pcb -> stack);

	cp -> esp (u32int)(proc5pcb -> stack_top);

	cp -> eip (u32int)proc5;

	cp -> eflags

}


