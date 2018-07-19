#include "keyboard_map.h"

extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern unsigned char keyboard_map[128];

char *vidptr = (char *)0xb8000;
unsigned int current_loc = 12;

void kmain(void) 
{
  const char *str = "kernel by vz";
  unsigned int i = 0;
  unsigned int j = 0;
  while (j < 80 * 25 * 2) {
    vidptr[j] = ' ';
    vidptr[j+1] = 0x07;
    j = j + 2;
  }
  j = 0;
  while (str[j] != '\0') {
    vidptr[i] = str[j];
    ++j;
    i = i + 2;
  }
  return ;
} 
void keyboard_handler_main(void) {
  unsigned char status;
  char keycode;
  write_port(0x20, 0x20);
  status = read_port(0x64);
  if (status & 0x01) {
    keycode = read_port(0x60);
    if(keycode<0) {
    return;
  }
    vidptr[current_loc++] = keyboard_map[keycode];
    vidptr[current_loc++] = 0x07;
  }
}

struct IDT_entry {
  unsigned short offset_lowerbits;
  unsigned short selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short offset_higherbits;
};

struct IDT_entry IDT[256];

void idt_init(void){
  unsigned long keyboard_address;
  unsigned long idt_address;
  unsigned long idt_ptr[2];
  keyboard_address = (unsigned long)keyboard_handler;
  IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
  IDT[0x21].selector = 0x08;
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = 0x8e;
  IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  write_port(0x20, 0x11);
  idt_address = (unsigned long)IDT;
  idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
  idt_ptr[1] = idt_address >> 16;
  load_idt(idt_ptr);
}

void kb_init(void) {
  write_port(0x21,0xFD);
}
