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
#include "modules/pcb.h"
#include "modules/r3commands.h"
#include "modules/context.h"


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
mpx_init(MODULE_R4);
 	
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


	//comhand();
	//asm volatile("int $60");
	loadr3();
	clearQueues();
	char comhandprocess[] = "comhand";
	load(comhandprocess, &comhand,0);
	pcb * comhandPCB = findPCB(comhandprocess);
	char repeat[] = "pos alpha.\n";
	int repeatSize = strlen(repeat);
	sys_req(WRITE, DEFAULT_DEVICE, repeat, &repeatSize);
	resume(comhandPCB);
	char idleprocess[] = "idle";
	load(idleprocess, &idle,9);
	pcb * idlePCB = findPCB(idleprocess);
	resume(idlePCB);
	char repeat2[] = "pos beta.\n";
	int repeatSize2 = strlen(repeat2);
	sys_req(WRITE, DEFAULT_DEVICE, repeat2, &repeatSize2);
	//showAllProcesses();
	asm volatile("int $60");



	//sys_req(READ,DEFAULT_DEVICE,buffer,&count);

   // 6) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
