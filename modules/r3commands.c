#include "r3commands.h"


void yield(){

	asm volatile("int $60");

}


void loadr3(){

	//create new pcb for PROCESS 1
	char * proc1Name = "Proc1";
	if(findPCB(proc1Name) == NULL){
		pcb * proc1pcb = setupPCB(proc1Name, 1, 9);	//char *PcbName, int classCode, int priorityCode
		suspend(proc1pcb);	//set to suspended ready

		context * cp1 = (context*)(proc1pcb -> top);
		memset(cp1, 0, sizeof(context));

		cp1 -> fs = 0x10;	
		cp1 -> gs = 0x10;
		cp1 -> ds = 0x10;
		cp1 -> es = 0x10;
		cp1 -> cs = 0x8;
		cp1 -> ebp = (u32int)(proc1pcb -> base);
		cp1 -> esp = (u32int)(proc1pcb -> top);
		cp1 -> eip = (u32int)proc1;//The function correlating to the process, ie. Proc1
		cp1 -> eflags = 0x202;


		//create new pcb for PROCESS 2
		char * proc2Name = "Proc2";
		pcb * proc2pcb = setupPCB(proc2Name, 1, 8);
		suspend(proc2pcb);	//set to suspended ready

		context * cp2 = (context*)(proc2pcb -> top);
		memset(cp2, 0, sizeof(context));

		cp2 -> fs = 0x10;
		cp2 -> gs = 0x10;
		cp2 -> ds = 0x10;
		cp2 -> es = 0x10;
		cp2 -> cs = 0x8;
		cp2 -> ebp = (u32int)(proc2pcb -> base);
		cp2 -> esp = (u32int)(proc2pcb -> top);
		cp2 -> eip = (u32int)proc2;
		cp2 -> eflags = 0x202;


		//create new pcb for PROCESS 3
		char * proc3Name = "Proc3";
		pcb * proc3pcb = setupPCB(proc3Name, 1, 7);
		suspend(proc3pcb);	//set to suspended ready

		context * cp3 = (context*)(proc3pcb -> top);
		memset(cp3, 0, sizeof(context));

		cp3 -> fs = 0x10;
		cp3 -> gs = 0x10;
		cp3 -> ds = 0x10;
		cp3 -> es = 0x10;
		cp3 -> cs = 0x8;
		cp3 -> ebp = (u32int)(proc3pcb -> base);
		cp3 -> esp = (u32int)(proc3pcb -> top);
		cp3 -> eip = (u32int)proc3;
		cp3 -> eflags = 0x202;


		//create new pcb for PROCESS 4
		char * proc4Name = "Proc4";
		pcb * proc4pcb = setupPCB(proc4Name, 1, 6);
		suspend(proc4pcb);	//set to suspended ready

		context * cp4 = (context*)(proc4pcb -> top);
		memset(cp4, 0, sizeof(context));

		cp4 -> fs = 0x10;
		cp4 -> gs = 0x10;	
		cp4 -> ds = 0x10;	
		cp4 -> es = 0x10;	
		cp4 -> cs = 0x8;
		cp4 -> ebp = (u32int)(proc4pcb -> base);
		cp4 -> esp = (u32int)(proc4pcb -> top);
		cp4 -> eip = (u32int)proc4;
		cp4 -> eflags = 0x202;
	
	
		//create new pcb for PROCESS 5
		char * proc5Name = "Proc5";
		pcb * proc5pcb = setupPCB(proc5Name, 1, 5);
		suspend(proc5pcb);	//set to suspended ready

		context * cp5 = (context*)(proc5pcb -> top);
		memset(cp5, 0, sizeof(context));

		cp5 -> fs = 0x10;
		cp5 -> gs = 0x10;
		cp5 -> ds = 0x10;
		cp5 -> es = 0x10;
		cp5 -> cs = 0x8;
		cp5 -> ebp = (u32int)(proc5pcb -> base);
		cp5 -> esp = (u32int)(proc5pcb -> top);
		cp5 -> eip = (u32int)proc5;
		cp5 -> eflags = 0x202;

	}else{
		char repeat[] = "There already exists a process with this name. Cannot create another one.\n";
		int repeatSize = strlen(repeat);
		sys_req(WRITE, DEFAULT_DEVICE, repeat, &repeatSize);
	}
}



