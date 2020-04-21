#include "context.h"
extern param params;
pcb *cop;
context *currentContext;

u32int* sys_call(context *registers){
	
	//char version[] = "\npos 1\n";
	//int versionSize = strlen(version);
	//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
	

	pcb *currentPcb;	
	currentPcb = getReadyQueueHead();
	if (cop == NULL){
		currentContext = registers;
		//strcpy(version,"pos 2\n");
		//int versionSize = strlen(version);
		//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
	}
	else {
		//strcpy(version,"params.op_code = ");
		//	int versionSize = strlen(version);
		//	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		//strcpy(version,intToAscii(params.op_code));
		//	versionSize = strlen(version);
		//	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		//strcpy(version,"   \n");
		//	versionSize = strlen(version);
		//	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);

		//if the opcode is EXIT, set to ready state and free the cop
		if (params.op_code == EXIT){
			//strcpy(version,"pos 3\n");
			//versionSize = strlen(version);
			//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
			cop -> stateRRB = 0;	//Ready state
			//testing one line below
				
			freePCB(cop);
		}

		//if the opcode is IDLE, set the top of the context to registers, set cop state to ready, and insert the cop
		else if (params.op_code == IDLE){
			/*strcpy(version,"HOWDY \n");
			int versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
			cop->top = (unsigned char*)registers;
			//testing one line below
			cop -> stateRRB = 0;
			//char version[] = "\npos 1\n";
			//strcpy(version,"HOWDY BRO \n");
			//int versionSize2 = strlen(version);
			//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize2);
			//showAllProcesses();
			insertPCB(cop);
			//showAllProcesses();
		}
		else{
			//strcpy(version,"pos 4.5\n");
			//int versionSize = strlen(version);
			//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		}
	}

	//cop = nextProcess();	
	

	//iif the current PCB is NULL, remove the PCB, set the currentPCB state to running, and set the current operating process to the currentPCB
	if (currentPcb != NULL){
		//strcpy(version,"pos 5\n");
		//int versionSize = strlen(version);
		//sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		removePCB(currentPcb);
		currentPcb -> stateRRB = 1;	//Running state
		cop = currentPcb;
		//strcpy(version,"pos 5.5\n");
		//	versionSize = strlen(version);
		//	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		//showAllProcesses();
		return (u32int*) cop->top;
	}

	//if the current PCB is NULL, return the current context
	else{
		//strcpy(version,"pos 6\n");
		//	int versionSize = strlen(version);
		//	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		return (u32int*) currentContext;
	}
}



