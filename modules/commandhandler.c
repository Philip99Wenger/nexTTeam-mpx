#include "commandhandler.h"

void version(){
	char version[] = "VERSION: 1.0";
	int versionSize = strlen(version);
	sys_req(WRITE, DEFAULT_DEVICE, version, &versionSize);
}

void help(){
	char help[] = "VERSION: 1.0";
	int helpSize = strlen(help);
	sys_req(WRITE, DEFAULT_DEVICE, help, &helpSize);
}

int comhand(){
char cmdBuffer[100];
int bufferSize;
int quit=0;
void (*version_ptr)() = &version;
void (*help_ptr)() = &help;
void (*shutdown_ptr)() = &shutdown;
void (*gettime_ptr)() = &gettime;
void (*settime_ptr)(int, int, int) = &settime;

while(!quit){
//get a command
memset(cmdBuffer, '\0', 100);
bufferSize = 99; //reset size before each call to read
sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

typedef void (*general_fp)(void);

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
	getDate,
	(general_fp)setDate
};

char *temp;
int i;
for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++){
	if(strcmp(cmdBuffer, commands[i]==0)){
		if(strcmp(commands[i]==commands[4])){
			//ask for parameters for setTime
		}else if(strcmp(commands[i]==commands[6])){
			//ask for parameters for setDate
		}else{
			*commands_ptrs[1])(); break;
		}
	}
}

//process the command
//see if quit was entered
}

return 1;
}

