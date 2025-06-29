#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/memory.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/apic.h>
#include <multiboot/multiboot.h>

void kernel_init(multiboot_info_t* multiboot_info) {
    setup_memory(multiboot_info);
    terminal_initialize();
    init_gdt();
    idt_init();
    PIC_remap(0x20, 0x28);
    enable_apic();
    io_apic_init();
}

void kernel_main(uint32_t magic, multiboot_info_t* multiboot_info) {
    if (magic != 0x2BADB002) {
        printf("Invalid magic number: %x\n", magic);
        return;
    }

    kernel_init(multiboot_info);

    uint32_t* num1 = kmalloc(sizeof(uint32_t), sizeof(uint32_t));

    *num1 = 69;

    printf("num1 addr: 0x%x\n", num1);

    printf("num1: %u\n", *num1);
}
