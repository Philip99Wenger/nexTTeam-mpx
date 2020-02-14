#include "commandhandler.h"
//qemu-system-i386 -nographic -kernel kernel.bin -s


//History Variables
char* thisHistory[10];//This contains 10 most recent commands
int index = 0;//This is a placeholder for history
char** historyArray = thisHistory;//this points to history

void version(){
	char version[] = "\x1B[36mVERSION: 2.0\x1B[37m\n";
	int versionSize = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
}

void help(){	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!TODO update with new commands !!!!!!!!!!!!!!!!!!!!!!!!!!!
	char help[] = "NAME\n     version - display current version of NTOS in use.\nDETAIL DESCRIPTION\n     No further description.\n\nNAME\n     getTime - display current time of system.\nDETAIL DESCRIPTION\n     Time will be displayed as hour:minute:second.\n\nNAME\n     setTime - change system's current time.\nDETAIL DESCRIPTION\n     Will prompt user to enter time as hh:mm:ss (i.e. hour:minute:second).\n\nNAME\n     getDate - display current date of system.\nDETAIL DESCRIPTION\n     Date will be displayed as month/day/year.\n\nNAME\n     setDate - change system's current date.\nDETAIL DESCRIPTION\n     Will prompt user to enter date as mm/dd/yy (i.e. month/day/year).\n\nNAME\n     shutdown - shuts down NTOS.\nDETAIL DESCRIPTION\n     Will prompt user to confirm system shut down as yes/no.\n\nNAME\n     suspend - places PCB in suspended state.\nDETAIL DESCRIPTION\n     Will prompt user for a process name and will set to suspended state and move to the appropriate queue.\n\nNAME\n     resume - places PCB in not suspended state.\nDETAIL DESCRIPTION\n     Will prompt user for a process name and will set to not suspended state and move to the appropriate queue.\n\nNAME\n     setPriority - sets PCB priority.\nDETAIL DESCRIPTION\n     Will prompt user for a process name and ask for new priority and will set to new priority and move to the appropriate queue.\n\nNAME\n     showPCB - display info for a PCB.\nDETAIL DESCRIPTION\n     Will ask for a process name and display the process name, class, state, suspended status, and priority for a PCB.\n\nNAME\n     showAllProcesses - display all PCBs.\nDETAIL DESCRIPTION\n     Will display the process name, class, state, suspended status, and priority for all PCBs.\n\nNAME\n     showReady - display all ready PCBs.\nDETAIL DESCRIPTION\n     Will display the process name, class, state, suspended status, and priority for all ready queue PCBs.\n\nNAME\n     showBlocked - display all blocked PCBs.\nDETAIL DESCRIPTION\n     Will display the process name, class, state, suspended status, and priority for all blocked PCBs.\n\nNAME\n     createPCB - create a new process.\nDETAIL DESCRIPTION\n     Will setup a PCB and insert in the appropriate queue.\n\nNAME\n     deletePCB - remove a PCB.\nDETAIL DESCRIPTION\n     Will find PCB, remove it from the queue, and free it.\n\nNAME\n     block - block a PCB.\nDETAIL DESCRIPTION\n     Will ask for a PCB name, block the process, and reinsert into the correct queue.\n\nNAME\n     unblock - unblock a PCB.\nDETAIL DESCRIPTION\n     Will ask for a PCB, unblock the process, and reinsert into the correct queue.\n\nNAME\n     history - print command history.\nDETAIL DESCRIPTION\n     Will display the user's ten previous commands.\n";
	int helpSize = strlen(help);
	
	sys_req(WRITE, DEFAULT_DEVICE, help, &helpSize);
}

int shutdown(){
	char shutdownBuffer[100];
	int bufferSize = 99;
	
	char confirm[] = "Are you sure you want to shut down?	yes/no\n";
	char stayInMenu[] = "\x1B[32mNot shutting down. Continue entering commands. \x1B[37m\n";
	int confirmSize = strlen(confirm);
	int stayInMenuSize = strlen(stayInMenu);
	
	//prompt user for confirmation
	sys_req(WRITE, DEFAULT_DEVICE, confirm, &confirmSize);
	
	//read user's input
	memset(shutdownBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, shutdownBuffer, &bufferSize);
	
	int returnValue;
	if(strcmp(shutdownBuffer, "yes") == 0){
		returnValue = 1;
	}else if(strcmp(shutdownBuffer, "no") == 0){
		sys_req(WRITE, DEFAULT_DEVICE, stayInMenu, &stayInMenuSize);
		returnValue = 0;
	}else{
		char shutdownError[] = "Please only enter the word 'yes' or 'no'.\n";
		int errorSize = strlen(shutdownError);
		sys_req(WRITE, DEFAULT_DEVICE, shutdownError, &errorSize);
		returnValue = 0;
	}
	
	return returnValue;

}

void settimeWrapper(){
	char timeBuffer[100];
	int bufferSize = 99;
	int parArr[3];
	int i=0;
	
	//prompt user for time to set to
	char prompt[] = "Enter time: hh:mm:ss\n";
	int promptSize = strlen(prompt);
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	
	memset(timeBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, timeBuffer, &bufferSize);

	char* token = strtok(timeBuffer, ":");
	while (token != NULL){
		parArr[i] = atoi(token);
		token = strtok(NULL, ":");
		i= i+1;
	}
	setTime(parArr[0],parArr[1],parArr[2]);
}

void setdateWrapper(){
	char dateBuffer[100];
	int bufferSize = 99;
	int parArr[3];
	int i=0;
	
	//prompt user for date to set to
	char prompt[] = "Enter date: mm/dd/yy\n";
	int promptSize = strlen(prompt);
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);

	memset(dateBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, dateBuffer, &bufferSize);

	char* token = strtok(dateBuffer, "/");
	while (token != NULL){
		parArr[i] = atoi(token);
		token = strtok(NULL, "/");
		i= i+1;
	}
	setDate(parArr[0],parArr[1],parArr[2]);
}


void suspendWrapper(){
	char suspendBuffer[100];
	int bufferSize = 99;

	//prompt user for pcb name
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char lenError[] = "The name is too long.\n";
	int lenErrorSize = strlen(lenError);
	char nameError[] = "There is no PCB by that name.\n";
	int nameErrorSize = strlen(nameError);
	pcb* PCB = NULL;
	
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	memset(suspendBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, suspendBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if(strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, lenError, &lenErrorSize);
		}else{
			PCB = findPCB(name);
			if(PCB == NULL){
				sys_req(WRITE, DEFAULT_DEVICE, nameError, &nameErrorSize);
			}else{
				suspend(PCB);
			}
		}
	}

}



void resumeWrapper(){
	char suspendBuffer[100];
	int bufferSize = 99;

	//prompt user for pcb name
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char lenError[] = "The name is too long.\n";
	int lenErrorSize = strlen(lenError);
	char nameError[] = "There is no PCB by that name.\n";
	int nameErrorSize = strlen(nameError);
	pcb* PCB = NULL;
	
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	memset(suspendBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, suspendBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if(strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, lenError, &lenErrorSize);
		}else{
			PCB = findPCB(name);
			if(PCB == NULL){
				sys_req(WRITE, DEFAULT_DEVICE, nameError, &nameErrorSize);
			}else{
				resume(PCB);
			}
		}
	}

}


/**TODO setPriorityWrapper

**/

//Interface for setupPCB
void createPCBWrapper(){
	//Variables	
	char pcbName[8];
	char *pcbNamePointer = pcbName;
	int pcbNameSize = 8;
	char pcbPriority[1];
	char *pcbPriorityPointer = pcbPriority;
	int pcbPrioritySize = 1;
	char pcbClass;
	int pcbClassSize = 1;
	char class[1];
	pcb* newPCB;
	pcb* sameName;

	//Prompts
	char namePrompt[] = "Enter the name for the new PCB that's no longer than 8 characters:\n";
	char *namePromptPointer = namePrompt;
	char priorityPrompt[] = "The PCB get's a priority from 0-9, with 9 going first.\nEnter the priority, a single integer from 0 to 9, of the new PCB:\n";
	char classPrompt[] = "Is this an application or a system process? enter either 'a' for application or 's' for system:\n";
	int promptSize;

	//Error or Success Messages
	char classError[] = "\x1B[31mInvalid entry. Please enter either 'a' or 's'.\x1B[37m\n";
	char priorityError[] = "\x1B[31mInvalid entry. Please enter an integer between '1' and '9'.\x1B[37m\n";
	char nameError[] = "\x1B[31mThis name is already taken. Please pick another.\x1B[37m\n";
	char success[] = "\x1B[32mValid Entry\x1B[37m\n";


	//Recieve Name
	promptSize = strlen(namePrompt);
	sys_req(WRITE, DEFAULT_DEVICE, namePromptPointer, &promptSize);
	memset(pcbName, '\0', 8);
	sys_req(READ, DEFAULT_DEVICE, pcbNamePointer, &pcbNameSize);
	sameName = findPCB(pcbName);
	if(sameName!=NULL){
		promptSize = strlen(nameError);
		sys_req(WRITE, DEFAULT_DEVICE, nameError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &promptSize);//Success!


	//Recieve Priority
	promptSize = strlen(priorityPrompt);
	sys_req(WRITE, DEFAULT_DEVICE, priorityPrompt, &promptSize);
	memset(&pcbPriority, '\0', 1);
	sys_req(READ, DEFAULT_DEVICE, pcbPriorityPointer, &pcbPrioritySize);
	if((*pcbPriority!='0')&&(*pcbPriority!='1')&&(*pcbPriority!='2')&&(*pcbPriority!='3')&&(*pcbPriority!='4')&&(*pcbPriority!='5')&&(*pcbPriority!='6')&&(*pcbPriority!='7')&&(*pcbPriority!='8')&&(*pcbPriority!='9')){
		promptSize = strlen(priorityError);
		sys_req(WRITE, DEFAULT_DEVICE, priorityError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &promptSize);//Success!
	

	//Recieve Class
	promptSize = strlen(classPrompt);
	sys_req(WRITE, DEFAULT_DEVICE, classPrompt, &promptSize);
	memset(class, '\0', 3);
	sys_req(READ, DEFAULT_DEVICE, class, &pcbClassSize);
	if(*class=='a'){
		pcbClass=1;
	} else if(*class=='s'){
		pcbClass=0;
	} else{
		promptSize = strlen(classError);
		sys_req(WRITE, DEFAULT_DEVICE, classError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(success);
	sys_req(WRITE, DEFAULT_DEVICE, success, &promptSize);//Success!

	//Make The PCB
	newPCB = setupPCB(pcbName, atoi(&pcbClass), atoi(pcbPriority));
	//Insert The PCB
	insertPCB(newPCB);
}

//Interface for find, remove, and freePCB
void deletePCBWrapper(){
	//Variables
	char pcbName[8];
	char *pcbNamePointer = pcbName;
	int pcbNameSize = 8;
	pcb* foundPCB;

	//Prompt And Error/Success
	char namePrompt[] = "Enter the name for the new PCB that's no longer than 8 characters:\n";
	char *namePromptPointer = namePrompt;
	char nameError[] = "\x1B[31mThis Process doesn't seem to exist.\x1B[37m\n";
	char nameSuccess[] = "\x1B[32mFound It!\x1B[37m\n";
	char removeError[] = "\x1B[31mEither something went wrong, or this PCB has already been removed.\x1B[37m\n";
	char removeSuccess[] = "\x1B[32mSuccessfully Removed!\x1B[37m\n";
	/**TODO: Uncomment this when FreePCB is completed
	char freeError[] = "\x1B[31mThere has been a failure to free memory associated with this PCB.\x1B[37m\n";
	char freeSuccess[] = "\x1B[32mMemory successfully freed!\x1B[37m\n";
	**/
	int promptSize;

	//Recieve Process Name
	promptSize = strlen(namePrompt);
	sys_req(WRITE, DEFAULT_DEVICE, namePromptPointer, &promptSize);
	memset(pcbName, '\0', 8);
	sys_req(READ, DEFAULT_DEVICE, pcbNamePointer, &pcbNameSize);
	foundPCB = findPCB(pcbName);
	if(foundPCB==NULL){
		promptSize = strlen(nameError);
		sys_req(WRITE, DEFAULT_DEVICE, nameError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(nameSuccess);
	sys_req(WRITE, DEFAULT_DEVICE, nameSuccess, &promptSize);//Success!

	//Remove It!
	if(removePCB(foundPCB)==-1){
		promptSize = strlen(removeError);
		sys_req(WRITE, DEFAULT_DEVICE, removeError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(removeSuccess);
	sys_req(WRITE, DEFAULT_DEVICE, removeSuccess, &promptSize);//Success!
	
	/**TODO: Uncomment this when FreePCB is completed	
	//Free Associated Memory
	if(FreePCB(foundPCB)==-1){
		promptSize = strlen(freeError);
		sys_req(WRITE, DEFAULT_DEVICE, freeError, &promptSize);
		return;//Error!
	}
	promptSize = strlen(freeSuccess);
	sys_req(WRITE, DEFAULT_DEVICE, freeSuccess, &promptSize);//Success!
	return;
	**/
}


void blockWrapper(){
	char blockBuffer[100];
	int bufferSize = 99;

	//prompt user for pcb name
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char lenError[] = "The name is too long.\n";
	int lenErrorSize = strlen(lenError);
	char nameError[] = "There is no PCB by that name.\n";
	int nameErrorSize = strlen(nameError);
	pcb* PCB = NULL;
	
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	memset(blockBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, blockBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if(strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, lenError, &lenErrorSize);
		}else{
			PCB = findPCB(name);
			if(PCB == NULL){
				sys_req(WRITE, DEFAULT_DEVICE, nameError, &nameErrorSize);
			}else{
				block(PCB);
			}
		}
	}

}


void unblockWrapper(){
	char blockBuffer[100];
	int bufferSize = 99;

	//prompt user for pcb name
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char lenError[] = "The name is too long.\n";
	int lenErrorSize = strlen(lenError);
	char nameError[] = "There is no PCB by that name.\n";
	int nameErrorSize = strlen(nameError);
	pcb* PCB = NULL;
	
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	memset(blockBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, blockBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if(strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, lenError, &lenErrorSize);
		}else{
			PCB = findPCB(name);
			if(PCB == NULL){
				sys_req(WRITE, DEFAULT_DEVICE, nameError, &nameErrorSize);
			}else{
				unblock(PCB);
			}
		}
	}
}

void setPriorityWrapper(){
	char showPCBBuffer[100];
	int bufferSize = 99;
	
	//prompt user for date to set to
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char priorityPrompt[] = "Enter the new priority for the PCB:\n";
	int priorSize = strlen(priorityPrompt);
	char error[] = "The name is too long.\n";
	int errSize = strlen(error);
	char priorError[] = "The priority number is not valid\n";
	int priorErrSize = strlen(priorError);
	int priorNum;
	
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
	memset(showPCBBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, showPCBBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if (strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, error, &errSize);
		}
		else{
			sys_req(WRITE, DEFAULT_DEVICE, priorityPrompt, &priorSize);
			memset(showPCBBuffer, '\0', 100);
			sys_req(READ, DEFAULT_DEVICE, showPCBBuffer, &bufferSize);
			char* token = strtok(NULL, "");
			while(token != NULL){
				priorNum = atoi(token);
				if(priorNum < 0 || priorNum > 9){
					sys_req(WRITE, DEFAULT_DEVICE, priorError, &priorErrSize);
				}
				else{
					setPriority(name, priorNum);
				}
			}
		}
	}
}

void showPCBWrapper(){
	char showPCBBuffer[100];
	int bufferSize = 99;
	
	//prompt user for date to set to
	char prompt[] = "Enter the PCB name:\n";
	int promptSize = strlen(prompt);
	char error[] = "The name is too long.\n";
	int errSize = strlen(error);

	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);

	memset(showPCBBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, showPCBBuffer, &bufferSize);

	char* name = strtok(NULL, "");
	while (name != NULL){
		if (strlen(name) > 8){
			sys_req(WRITE, DEFAULT_DEVICE, error, &errSize);
		}
		else{
			showPCB(name);
		}
	}
}

void historyWrapper(){
	int i;
	for(i=0; i<10; i++){//only print out ten commands
		int textSize = strlen(thisHistory[index]);//size of this command
		sys_req(WRITE, DEFAULT_DEVICE, thisHistory[index], &textSize);//print out this command	
		index++;//increment the placeholder
		if(index>=10){index=0;}//loop back to the beginning of the array if at the end
		//This will print out ten commands, from oldest to most recent.
	}
}

int comhand(){
	char cmdBuffer[100];
	int bufferSize;
	int quit=0;
	unsigned int i;
	void (*version_ptr)() = &version;
	void (*help_ptr)() = &help;
	void (*gettime_ptr)() = &gettime;
	void (*settime_ptr)() = &settimeWrapper;
	void (*getdate_ptr)() = &getdate;
	void (*setdate_ptr)() = &setdateWrapper;
	//R2 functions -- uncomment as implemented
	//void (*suspend_ptr)() = &suspendWrapper;
	//void (*resume_ptr)() = &resumeWrapper;
	void (*setPriority_ptr)() = &setPriorityWrapper;
	void (*showPCB_ptr)() = &showPCBWrapper;
	void (*showAllProcess_ptr)() = &showAllProcesses;
	void (*showReady_ptr)() = &showReady;
	void (*showBlocked_ptr)() = &showBlocked;
	void (*createPCB)() = &createPCBWrapper;
	void (*deletePCB)() = &deletePCBWrapper;
	void (*block)() = &blockWrapper;
	void (*unblock)() = &unblockWrapper;
	void (*history)() = &historyWrapper;

	char commands[19][20]={
		"shutdown", //must keep shutdown at index 0
		"version",
		"help",
		"getTime",
		"setTime",
		"getDate",
		"setDate",
		"suspend", //R2 functions vvvvv
		"resume",
		"setPriority",
		"showPCB",
		"showAllProcesses",
		"showReady",
		"showBlocked",
		"createPCB",
		"deletePCB",
		"block",
		"unblock",
		"history"
	};
	void (*commands_ptrs[])()={
		*version_ptr,
		*help_ptr,
		*gettime_ptr,
		*settime_ptr,
		*getdate_ptr,
		*setdate_ptr,
		//*suspend_ptr,
		//*resume_ptr,
		*setPriority_ptr,
		*showPCB_ptr,
		*showAllProcess_ptr,
		*showReady_ptr,
		*showBlocked_ptr,
		*createPCB,
		*deletePCB,
		*block,
		*unblock,
		*history
	};
	//Print fancy menu
	char nextTeam[] = "\x1B[33mX   X  XXXX  X   X  XXXXX    XXXXX  XXXX    X    X   X\nXX  X  X      X X     X        X    X      X X   XX XX\nX X X  XXX     X      X        X    XXX   X   X  X X X\nX  XX  X      X X     X        X    X     XXXXX  X   X\nX   X  XXXX  X   X    X        X    XXXX  X   X  X   X\n";
	int nextTeamSize = strlen(nextTeam);
	sys_req(WRITE, DEFAULT_DEVICE, nextTeam, &nextTeamSize);
	char mickey[] = "\x1B[34mT       XXXX      XXXX       \nE      XXXXXX    XXXXXX      \nL       XXXX      XXXX       \nE        XXXXXXXXXXXX        \nT       XX\x1B[37mXXXXXXXXXX\x1B[34mXX       \nU      XX\x1B[37mXXXXXXXXXXXX\x1B[34mXX      \nB     XX\x1B[37mXXXXXXXXXXXXXX\x1B[34mXX     \nB      XX\x1B[37mXXXXXXXXXXXX\x1B[34mXX      \nI       XX\x1B[37mXXXXXXXXXX\x1B[34mXX       \nE        XXXXXXXXXXXX\x1B[37m        \n";
	int mickeySize = strlen(mickey);
	sys_req(WRITE, DEFAULT_DEVICE, mickey, &mickeySize);
	
	//Print welcome message
	char welcome[] = "Welcome to NextTeam's OS!\nPlease type one of the available commands:\n";
	int welcomeSize = strlen(welcome);
	sys_req(WRITE, DEFAULT_DEVICE, welcome, &welcomeSize);	
		
	int tempSize = 20;
	char sloppyTemp[] = "\n";
	int sloppySize = 2;
	for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++){
		sys_req(WRITE, DEFAULT_DEVICE, commands[i], &tempSize);
		sys_req(WRITE, DEFAULT_DEVICE, sloppyTemp, &sloppySize);
	}	

	//Set History to Null
	for(index = 0; index < 10; index++){thisHistory[index]= '\0';}
	index = 0; //Set the history placeholder
	
	while(!quit){
		//get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99; //reset size before each call to read
		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
		
		//history
		thisHistory[index] = cmdBuffer;//put the called command into history
		if(index>=9){index=0;}else{index++;}//increment the history placeholder
		
		//
		int shutdownVal;
		for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++){
			if(strcmp(cmdBuffer, commands[i])==0){
				if(i == 0){
					shutdownVal = shutdown();
					if(shutdownVal == 1){quit = 1;}
				}else{(*commands_ptrs[i-1])();}	
			}
		}
	}
	
	return 1;
}
	
