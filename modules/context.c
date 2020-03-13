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
		/*strcpy(version,"pos 2\n");
		int versionSize = strlen(version);
		sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
	}
	else {
		/*strcpy(version,"params.op_code = ");
			int versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		strcpy(version,intToAscii(params.op_code));
			versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
		strcpy(version,"   \n");
			versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
		if (params.op_code == EXIT){
			/*strcpy(version,"pos 3\n");
			versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
			cop -> stateRRB = 0;	//Ready state
			//testing one line below
				
			freePCB(cop);
		}
		else if (params.op_code == IDLE){
			/*strcpy(version,"pos 4\n");
			int versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
			cop->top = (unsigned char*)registers;
			//testing one line below
			cop -> stateRRB = 0;
			
			insertPCB(cop);
		}
		else{
			/*strcpy(version,"pos 4.5\n");
			int versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
		}
	}

	//cop = nextProcess();	

	if (currentPcb != NULL){
		/*strcpy(version,"pos 5\n");
		int versionSize = strlen(version);
		sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
		removePCB(currentPcb);
		currentPcb -> stateRRB = 1;	//Running state
		cop = currentPcb;
		
		return (u32int*) cop->top;
	}
	else{
		/*strcpy(version,"pos 6\n");
			int versionSize = strlen(version);
			sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);*/
		return (u32int*) currentContext;
	}
}
