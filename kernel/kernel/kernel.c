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
}
