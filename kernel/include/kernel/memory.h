#ifndef MEMORY_H
#define MEMORY_H

#include <multiboot/multiboot.h> 
#include <stdint.h>

#define PAGE_SIZE 4096
#define BITMAP_SIZE 32768

extern uint8_t page_bitmap[BITMAP_SIZE];
extern uint8_t _kernel_start;
extern uint8_t _endkernel;

void setup_memory(multiboot_info_t* mbinfo);

uint32_t alloc_page();

#endif
