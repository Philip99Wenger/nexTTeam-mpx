#include "commandhandler.h"

int comhand(){
char cmdBuffer[100];
int bufferSize;
int quit=0;

white(!quit){
//get a command
memset(buffer, '\0', 100);
bufferSize = 99; //reset size before each call to read
sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

//process the command
//see if quit was entered
}

}
