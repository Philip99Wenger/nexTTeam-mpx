#include r3commands.h

void yield(){
	asm volatile("int $60");
}

void loadr3(){
	//DEFINE STACK_SIZE!!!!!!!

	//create new pcb for PROCESS 1
	char * proc1Name = "Proc1";
	pcb * proc1 = setup(proc1Name, 1, 1, 9);	//char *PcbName, int classCode, int priorityCode
	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!
	proc1->base = sys_alloc_mem(stack_size);
	proc1->top = (proc1->base) + 1024 - sizeof(struct context);
	suspend(proc1);	//set to suspended ready
	context * cp = (context*)(proc1 -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(proc1 -> stack);
	cp -> esp (u32int)(proc1 -> stack_top);
	cp -> eip (u32int)Proc1;//The function correlating to the process, ie. Proc1
	cp -> eflags

	//create new pcb for PROCESS 2
	char * proc2Name = "Proc2";
	pcb * proc2 = setup(proc2Name, 1, 1, 8);
	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!
	proc2->base = sys_alloc_mem(stack_size);
	proc2->top = (proc2->base) + 1024 - sizeof(struct context);
	context * cp = (context*)(proc2 -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(proc2 -> stack);
	cp -> esp (u32int)(proc2 -> stack_top);
	cp -> eip (u32int)Proc2;
	cp -> eflags

	//create new pcb for PROCESS 3
	char * proc3Name = "Proc3";
	pcb * proc3 = setup(proc3Name, 1, 1, 7);
	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!
	proc3->base = sys_alloc_mem(stack_size);
	proc3->top = (proc3->base) + 1024 - sizeof(struct context);
	context * cp = (context*)(proc3 -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(proc3 -> stack);
	cp -> esp (u32int)(proc3 -> stack_top);
	cp -> eip (u32int)Proc3;
	cp -> eflags

	//create new pcb for PROCESS 4
	char * proc4Name = "Proc4";
	pcb * proc4 = setup(proc4Name, 1, 1, 6);
	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!
	proc4->base = sys_alloc_mem(stack_size);
	proc4->top = (proc4->base) + 1024 - sizeof(struct context);
	context * cp = (context*)(proc4 -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(proc4 -> stack);
	cp -> esp (u32int)(proc4 -> stack_top);
	cp -> eip (u32int)Proc4;
	cp -> eflags

	//create new pcb for PROCESS 5
	char * proc5Name = "Proc5";
	pcb * proc5 = setup(proc5Name, 1, 1, 5);
	//!!!!!!!!!!!!review pcb's top and bottom setup!!!!!!!!!!!!!!!!!!!
	proc5->base = sys_alloc_mem(stack_size);
	proc5->top = (proc5->base) + 1024 - sizeof(struct context);
	context * cp = (context*)(proc5 -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(proc5 -> stack);
	cp -> esp (u32int)(proc5 -> stack_top);
	cp -> eip (u32int)Proc5;
	cp -> eflags
}
