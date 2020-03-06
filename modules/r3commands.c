#include r3commands.h

void yield(){
	asm volatile("int $60");
}

void loadr3(){
	pcb * new_pcb = create_pcb (name, 1,1,1,stack_size);
	context * cp = (context*)(new_pcb -> stack_top);
	memeset(cp, 0, sizeof(context));
	cp -> fs = 0 x10;
	cp -> gs = 0 x10;
	cp -> ds = 0 x10;
	cp -> es = 0 x10;
	cp -> cs = 0 x8;
	cp -> ebp (u32int)(new_pcb -> stack);
	cp -> esp (u32int)(new_pcb -> stack_top);
	cp -> eip (u32int)func;//The function correlating to the process, ie. Proc1
	cp -> eflags
	return new_pcb;
}
