#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/memory.h>
#include <multiboot/multiboot.h>

void kernel_main(uint32_t magic, multiboot_info_t* multiboot_info) {
    terminal_initialize();

    if (magic != 0x2BADB002) {
        printf("Invalid magic number: %x\n", magic);
        return;
    }

    setup_memory(multiboot_info);
    printf("Paging Enabled!\n");

     uint32_t* num1 = kmalloc(sizeof(uint32_t), sizeof(uint32_t));

     *num1 = 696969;

     uint32_t* num2 = kmalloc(sizeof(uint32_t), sizeof(uint32_t));

     *num2 = 12345;

     printf("num1 addr: %x\n", num1);
     printf("num1: %u\n", *num1);
     printf("num2 addr: %x\n", num2);
     printf("num2: %u\n", *num2);
}
