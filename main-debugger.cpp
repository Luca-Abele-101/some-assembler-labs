#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <iostream.h>

void interrupt (*old)(...);

int i = 0;

void dummy()
{
  asm {
    mov ax, 0
  }

  asm {
    mov ax, 1
    mov cx, 10
  }
  l:
  asm {
    add ax, 1
    loop l
  }

  asm {
    mov ax, 0
  }

  disable();
  setvect(0x01, old);
  enable();
}

void interrupt debugger(...)
{
  asm {
    cli
  }
  if (i > 3)
  {
  cout << "AX: " << _AX << "\n";
  char c = getch();
  }
  i++;
  if ( i >= 23)
  {
    cout << "End of Program";
    getch();
    exit(0);
  }
  asm {
    sti
  }
}

void main(void)
{
  clrscr();
  disable();
  old = getvect(0x01);
  setvect(0x01, debugger);
  cout << "Program has started\n";
  asm {
    pushf

    pushf
    pop ax
    or ax, 0x100
    push ax

    push cs

    lea ax, dummy
    push ax
    iret
  }
}
