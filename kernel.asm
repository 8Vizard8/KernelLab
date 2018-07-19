bits 32
section .text
  ;multiboot spec
  align 4
  dd 0x1BADB002
  dd 0x00
  dd - (0x1BADB002)

global start
global keyboard_handler
global read_port
global write_port
global load_idt

extern kmain
extern keyboard_handler_main

start:
  cli
  mov esp, stack_space
  call kmain
  hlt
read_port:
  mov edx, dword[esp + 4]
  in al, dx
  ret
write_port:
  mov edx, dword[esp + 4]
  mov al, byte[esp + 8]
  out dx, al
  ret
load_idt:
  mov edx, dword[esp+4]
  lidt [edx]
  sti
  ret
keyboard_handler:
  call keyboard_handler_main
  iretd
section .bss
resb 8192
stack_space:
