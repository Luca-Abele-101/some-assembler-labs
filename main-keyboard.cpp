#include <iostream.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>

char scanCode;

void interrupt (*oldHandler)(...);
void interrupt handler(...)
{
	asm in al, 0x60
	scanCode = _AL;
	printf("ScanCode = %d\n", scanCode);
	oldHandler();
}

void main()
{
	clrscr();

	cout << "\nTask:\n";
	cout << "\nPress any key to see the result\n";
	cout << "or press SPACE to continue.\n\n";
	char cScan,cAsci;
	int iAdres;

	do
	{
	asm {
		push ds
		push si
		push di
		mov ax, 0x40
		mov ds, ax
		mov si, 0x1A
		mov di, 0x1C
		}
	wait_kbd:
	asm {
		 mov ax, [si]
		 cmp ax, [di]
		 je wait_kbd

		 mov si, [si]
	    }
		iAdres = _SI;
		asm mov ax, [si]
		asm pop di
		asm pop si
		asm pop ds
		cScan = _AH;
		cAsci = _AL;
		printf("Address = %x    ScanCode = %d     ASCII = %d \n", iAdres, cScan, cAsci);
	} while (getch() != 32);

	cout << "\nThe end of task. Press any key...\n";
	getch();

	delay(2000);
	asm in al,0x60
	cScan=0;
	printf("Scan = %x \n", cScan);
}
