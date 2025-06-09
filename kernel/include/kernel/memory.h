#ifndef MEMORY_H
#define MEMORY_H

#include <multiboot/multiboot.h> 
#include <stdint.h>

#define PAGE_SIZE 4096
#define BITMAP_SIZE 32768
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024

#define KERNEL_HEAP_START 0xC0000000
#define KERNEL_HEAP_SIZE 16 * 1024 * 1024

extern uint8_t _kernel_start;
extern uint8_t _endkernel;

void setup_memory(multiboot_info_t* mbinfo);

uint32_t alloc_page();
void free_page(size_t page);

#endif
