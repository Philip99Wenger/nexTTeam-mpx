#ifndef _CONTEXT_H
#define _CONTEXT_H

typedef struct context{
	u32int gs, fs, es, ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int eip, cs, eflags;
}

#endif
