/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/polling.h"
#include "modules/commandhandler.h"
#include "modules/date.h"



//STRUCTS

//This is a process//
typedef struct{ 
	char processName[8];		//The name of the process
	char *namePtr;	//Pointer to the name
	int priority;			//0-9 ranking of importance, with 9 going first
	int stateRRB;			//Ready(0)/Running(1)/Blocked(3)
	int stateIsSuspended;		//Suspended(1)/Not-Suspended(0)
	int classIsApp;			//Application(1)/System-Process(0)
	unsigned char* pcbNext;		//Pointer to the start of the next process
	unsigned char* pcbPrev;		//Pointer to the start of the last process

	//This is the stack//	
	unsigned char* base;		//Pointer to the base of the process
	unsigned char* top;		//Pointer to the top of the process, or base + 1024 bytes
	//1024 Bytes, Every Character should be initialized to null//
} pcb;
//This is the ready queue//
typedef struct{
	int count;	//Number of processes in the queue
	pcb *head;	//Pointer to the first pcb in the queue, the first one in order, highest priority
	pcb *tail;	//Pointer to the last pcb in the queue, the last one in order, lowest priority
} readyQueue;
//This is the blocked queue//
typedef struct{
	int count;	//Number of processes in the queue
	pcb *head;	//Pointer to the first pcb in the queue, the first one in order
	pcb *tail;	//Pointer to the last pcb in the queue, the last one in order
} blockedQueue;
//This is the suspended-ready queue//
typedef struct{
	int count;	//Number of processes in the queue
	pcb *head;	//Pointer to the first pcb in the queue, the first one in order, highest priority
	pcb *tail;	//Pointer to the last pcb in the queue, the last one in order, lowest priority
} suspendReadyQueue;
//This is the suspended-blocked queue//
typedef struct{
	int count;	//Number of processes in the queue
	pcb *head;	//Pointer to the first pcb in the queue, the first one in order
	pcb *tail;	//Pointer to the last pcb in the queue, the last one in order
} suspendBlockedQueue;




void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   // 0) Initialize Serial I/O and call mpx_init


 
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
init_serial(0x3f8);
set_serial_out(0x3f8);
set_serial_in(0x3f8);
mpx_init(MODULE_R1);
 	
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
   
   // 3) Descriptor Tables
   klogv("Initializing descriptor tables...");
init_gdt();
init_idt();
init_pic();
init_irq();
sti();
init_paging();


   // 4) Virtual Memory
   klogv("Initializing virtual memory...");


   // 5) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");
	//typedef void(*polptr)(char * buffer, int * count);
	//void* pollptr=poll
	sys_set_read(&poll);
	//char buffer[100];
	//int count=100;
	comhand();

	//sys_req(READ,DEFAULT_DEVICE,buffer,&count);

   // 6) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
