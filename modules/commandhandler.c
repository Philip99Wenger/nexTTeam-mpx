#include "commandhandler.h"

void version(){
	char version[] = "VERSION: 1.0";
	int versionSize = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
}

void help(){
	char help[] = "version	-- displays version of system\ngetTime	-- displays current time of system\nsetTime -- sets current time of system\ngetTime	-- displays current date of system\nsetTime -- sets current date of system";
	int helpSize = strlen(help);
	sys_req(WRITE, DEFAULT_DEVICE, help, &helpSize);
}

void shutdown(){

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
	char prompt[] = "Enter date: mm/dd/yy";
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
void (*shutdown_ptr)() = &shutdown;
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
	"shutdown",
	"getTime",
	"setTime",
	"getDate",
	"setDate"
};

void (*commands_ptrs[])()={
	*version_ptr,
	*help_ptr,
	*shutdown_ptr,
	*gettime_ptr,
	*settime_ptr,
	*getdate_ptr,
	*setdate_ptr
};

//char *temp;
unsigned int i;
for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++){
	if(strcmp(cmdBuffer, commands[i])==0){
//		if(strcmp(commands[i], commands[4]) == 0){
//			//ask for parameters for setTime
//		}else if(strcmp(commands[i],commands[6]) == 0){
//			//ask for parameters for setDate
//		}else{
//			*commands_ptrs[1](); break;
//		}
		(*commands_ptrs[i])();
	}
}
}

return 1;
}

