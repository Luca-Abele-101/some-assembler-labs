#include <stdio.h>
#include <mem.h>
#include <dos.h>
#include <conio.h>
#include <bios.h>
#include <stdlib.h>

struct ProcStats // Context data struct
{
	unsigned int rax, rbx, rcx, rdx; // 0  2  4  6
	unsigned int rsi, rdi, rbp, rsp; // 8  10 12 14
	unsigned int rcs, rds, res, rss; // 16 18 20 22
	unsigned int rip, rflags, a, b;  // 24 26 28 30
}
Stats[3];

unsigned int current_proc;  // Current task number
unsigned int stats_offset;  // Context address
void interrupt (*oldHandler)(...);
void interrupt (*oldKbdHandler)(...);

void interrupt kbdHandler(...)
{
	sleep(1);
	printf("\nThe process has been stopped... Exit program\n");
	sleep(2);
	exit(0);
}

void interrupt IntHandler(...)
{
    asm {
	mov si, [current_proc] // Process number
	mov cl, 5
	shl si, cl
	mov ax, [stats_offset]
	add si, ax  // context address -> si

	// Registers -> Context :
	pop ax
	mov [si + 12], ax // bp
	pop ax
	mov [si + 10], ax // di
	pop ax
	mov [si + 8], ax // si
	pop ax
	mov [si + 18], ax // ds
	pop ax
	mov [si + 20], ax // es
	pop ax
	mov [si + 6], ax // dx
	pop ax
	mov [si + 4], ax // cx
	pop ax
	mov [si + 2], ax // bx
	pop ax
	mov [si + 0], ax // ax
	pop ax
	mov [si + 24], ax // ip
	pop ax
	mov [si + 16], ax // cs
	pop ax
	mov [si + 26], ax // flags
	mov ax, sp
	mov [si + 14], ax // sp
	mov ax, ss
	mov [si + 22], ax // ss

	mov ax, [current_proc]  // Next task
	inc ax
	cmp ax, 3
	jb label
	mov ax, 0
	}
	label:
	asm {
		mov [current_proc], ax  // Counting task2  context address
		mov si, ax
		mov cl, 5
		shl si, cl
		mov ax, [stats_offset]
		add si, ax

		mov ax, [si + 22] // ss
		mov ss, ax
		mov ax, [si + 14] // sp
		mov sp, ax
		mov ax, [si + 26] // flags
		push ax
		mov ax, [si + 16] // cs
		push ax
		mov ax, [si + 24] // ip
		push ax
		mov ax, [si + 0] // ax
		push ax
		mov ax, [si + 2] // bx
		push ax
		mov ax, [si + 4] // cx
		push ax
		mov ax, [si + 6] // dx
		push ax
		mov ax, [si + 20] // es
		push ax
		mov ax, [si + 18] // ds
		push ax
		mov ax, [si + 8] // si
		push ax
		mov ax, [si + 10] // di
		push ax
		mov ax, [si + 12] // bp
		push ax
		}
}

void Proc1(); void Proc2(); void Proc3();

int main()
{
 unsigned int i, rd, rc, re, rs, ri0, ri1, ri2, f;
 for (i = 0; i < 3; ++i)
 {
	memset(&Stats[i], 0, sizeof(ProcStats));
 }
 asm {               // Registers -> Temporary Variables
	mov ax, cs
	mov [rc], ax
	mov ax, ds
	mov [rd], ax
	mov ax, es
	mov [re], ax
	mov ax, ss
	mov [rs], ax
	mov [ri0], offset Proc1
	mov [ri1], offset Proc2
	mov [ri2], offset Proc3
	pushf
	pop ax
	mov [f], ax
	}
for (i = 0; i < 3; ++i)  // Filling Contexts with sr's and f's
{
	Stats[i].rcs = rc;
	Stats[i].rds = rd;
	Stats[i].res = re;
	Stats[i].rss = rs;
	Stats[i].rflags = f;
}
	Stats[0].rip = ri0;
	Stats[1].rip = ri1;
	Stats[2].rip = ri2;
	current_proc = 0; // Current process is #1
	stats_offset = (unsigned) Stats;
	oldHandler = getvect(0xF0);
	setvect(0xF0, IntHandler);
	oldKbdHandler = getvect(0x9);
	setvect(0x9, kbdHandler);
	Proc1();
	setvect(0xF0, oldHandler);
	setvect(0x9, oldKbdHandler);

	return 0;
}

unsigned int clk = 0;

void Proc1()
{
	while(1)
	{
		printf("Process #1 is working...\n");
		sleep(1);
		if (clk == 2)
		{
			clk = 0;
			geninterrupt(0xF0);
		}
		else
		{
			++clk;
		}
	}
}
void Proc2()
{
	while(1)
	{
		printf("Process #2 is working...\n");
		sleep(1);
		if (clk == 2)
		{
			clk = 0;
			geninterrupt(0xF0);
		}
		else
		{
			++clk;
		}
	}
}
void Proc3()
{
	while(1)
	{
		printf("Process #3 is working...\n");
		sleep(1);
		if (clk == 2)
		{
			clk = 0;
			geninterrupt(0xF0);
		}
		else
		{
			++clk;
		}
	}
}
