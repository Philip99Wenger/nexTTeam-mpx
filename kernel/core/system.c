#include <string.h>
#include <system.h>

#include <core/serial.h>

#include <modules/mpx_supt.h>
#include <core/io.h>
#include <modules/pcb.h>

pcb *cop;
context *currentContext;

/*
  Procedure..: klogv
  Description..: Kernel log messages. Sent to active
      serial device.
*/
void klogv(const char *msg)
{
  char logmsg[64] = {'\0'}, prefix[] = "klogv: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  serial_println(logmsg);
}

/*
  Procedure..: kpanic
  Description..: Kernel panic. Prints an error message
      and halts.
*/
void kpanic(const char *msg)
{
  cli(); //disable interrupts
  char logmsg[64] = {'\0'}, prefix[] = "Panic: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  klogv(logmsg);
  hlt(); //halt
}

u32int* sys_call(context *registers){
	pcb *currentPcb = getReadyQueueHead();
	if (cop == NULL){
		currentContext = registers;
        }
	else {
		if (params.op_code == EXIT){
			freePCB(cop);
		}
		if (params.op_code ==  IDLE){
			pcb->top = (unsigned char*)registers;
			insertPCB(cop);
		}
	} 
	if (currentPcb != NULL){
		cop = currentPcb;
		removePCB(currentPcb);
		return (u32int*) cop->top;
	}
	else{
		return (u32int*) currentContext;
	}
}

