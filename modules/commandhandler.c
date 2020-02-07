#include "commandhandler.h"

void version(){
	char version[] = "VERSION: 1.0\n";
	int versionSize = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
}

void help(){

//command descriptions
//	char versionD[] = "NAME\n     version - display current version of NTOS in use.\nDETAIL DESCRIPTION\n     No further description.";
//	int versionDSize = strlen(versionD);
//
//	char getTimeD[] = "NAME\n     getTime - display current time of system.\nDETAIL DESCRIPTION\n     Time will be displayed as hour:minute:second.";
//	int getTimeDSize = strlen(getTimeD);
//
//	char setTimeD[] = "NAME\n     setTime - change system's current time.\nDETAIL DESCRIPTION\n     Will prompt user to enter time as hh:mm:ss (i.e. hour:minute:second).";
//	int setTimeDSize = strlen(setTimeD);
//
//	char getDateD[] = "NAME\n     getDate - display current date of system.\nDETAIL DESCRIPTION\n     Date will be displayed as month/day/year.";
//	int getDateDSize = strlen(getDateD);
//
//	char setDateD[] = "NAME\n     setDate - change system's current time.\nDETAIL DESCRIPTION\n     Will prompt user to enter date as mm/dd/yy (i.e. month/day/year)."; 
//	int setDateDSize = strlen(setDateD);
//
//
//	char shutdownD[]= "NAME\n     shutdown - shuts down NTOS.\nDETAIL DESCRIPTION\n     Will prompt user to confirm system shut down as yes/no."; 
//	int shutdownDSize = strlen(shutdownD);

//help menu
	char helpBuffer[100];
	int bufferSize = 99;

	char help[6][15] = {"version\n","getTime\n","setTime\n","getDate\n","setDate\n", "shutdown\n"};
	int helpSize = 14;
	char help2[6][15] = {"version","getTime","setTime","getDate","setDate", "shutdown"};

	char helpDescriptions[6][200] = {
		"NAME\n     version - display current version of NTOS in use.\nDETAIL DESCRIPTION\n     No further description.\n",
		"NAME\n     getTime - display current time of system.\nDETAIL DESCRIPTION\n     Time will be displayed as hour:minute:second.\n",
		"NAME\n     setTime - change system's current time.\nDETAIL DESCRIPTION\n     Will prompt user to enter time as hh:mm:ss (i.e. hour:minute:second).\n",
		"NAME\n     getDate - display current date of system.\nDETAIL DESCRIPTION\n     Date will be displayed as month/day/year.\n",
		"NAME\n     setDate - change system's current time.\nDETAIL DESCRIPTION\n     Will prompt user to enter date as mm/dd/yy (i.e. month/day/year).\n",
		"NAME\n     shutdown - shuts down NTOS.\nDETAIL DESCRIPTION\n     Will prompt user to confirm system shut down as yes/no.\n"
	};
	
	char helpPrompt[] = "Please enter command for more information.\n";
	int helpPromptSize = strlen(helpPrompt);
	
	//writes prompt
	sys_req(WRITE, DEFAULT_DEVICE, helpPrompt, &helpPromptSize);

	//writes command options
	unsigned int i;
	for(i=0; i<sizeof(help2)/sizeof(help2[0]); i++){
		sys_req(WRITE, DEFAULT_DEVICE, help[i], &helpSize);
	}

	//read which command
	memset(helpBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, helpBuffer, &bufferSize);
	
	int temp;
	for(i=0; i<sizeof(help2)/sizeof(help2[0]); i++){
		if(strcmp(helpBuffer, help2[i]) == 0){	
			temp = strlen(helpDescriptions[i]);	
			sys_req(WRITE, DEFAULT_DEVICE, helpDescriptions[i], &temp);
		}
	}		
}

int shutdown(){
	char shutdownBuffer[100];
	int bufferSize = 99;
	
	char confirm[] = "Are you sure you want to shut down?	yes/no\n";
	int confirmSize = strlen(confirm);
	
	//prompt user for confirmation
	sys_req(WRITE, DEFAULT_DEVICE, confirm, &confirmSize);
	
	//read user's input
	memset(shutdownBuffer, '\0', 100);
	sys_req(READ, DEFAULT_DEVICE, shutdownBuffer, &bufferSize);
	
	int returnValue;
	if(strcmp(shutdownBuffer, "yes") == 0){
		returnValue = 1;
	}else if(strcmp(shutdownBuffer, "no") == 0){
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
	char prompt[] = "Enter time: hh:mm:ss";
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

int comhand(){
char cmdBuffer[100];
int bufferSize;
int quit=0;
void (*version_ptr)() = &version;
void (*help_ptr)() = &help;
void (*gettime_ptr)() = &gettime;
void (*settime_ptr)() = &settimeWrapper;
void (*getdate_ptr)() = &getdate;
void (*setdate_ptr)() = &setdateWrapper;

while(!quit){
//get a command
memset(cmdBuffer, '\0', 100);
bufferSize = 99; //reset size before each call to read
sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

char commands[7][15]={
	"version",
	"help",
	"getTime",
	"setTime",
	"getDate",
	"setDate",
	"shutdown"
};

void (*commands_ptrs[])()={
	*version_ptr,
	*help_ptr,
	*gettime_ptr,
	*settime_ptr,
	*getdate_ptr,
	*setdate_ptr
};

unsigned int i;
int shutdownVal;
for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++){
	if(strcmp(cmdBuffer, commands[i])==0){
		if(i == 6){
			shutdownVal = shutdown();
			if(shutdownVal == 1){quit = 1;}
		}else{
			(*commands_ptrs[i])();
		}	
	}
}
}

return 1;
}

