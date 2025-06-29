#ifndef MEMORY_H
#define MEMORY_H

#include <multiboot/multiboot.h>
#include <stdint.h>

#define HEAP_START 0XC1000000

#define PAGE_SIZE 4096
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024

// Page Flags
#define PRESENT          0x01
#define READ_WRITE       0x02
#define USER             0x04
#define WRITE_THROUGH    0x08
#define CACHE_DISABLE    0x10
#define ACCESSED         0x20
#define AVAILABLE        0x40
#define PS               0x80
#define DIRTY            0x40

extern uint8_t _kernel_start;
extern uint8_t _endkernel;

void setup_memory(multiboot_info_t* mbinfo);

void reserve_page(uintptr_t addr);

void map_page(uintptr_t virtual_addr, uintptr_t phys_addr, uint32_t flags);

void reserve_region(uintptr_t addr, uint32_t size);

void map_region(uintptr_t virtual_addr, uintptr_t phys_addr, uint32_t size, uint32_t flags);

void* kmalloc(uint32_t size, uint32_t alignment);

#endif
