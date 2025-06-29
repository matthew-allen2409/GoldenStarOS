#include <kernel/idt.h>
#include <stdio.h>

static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idt_ptr_t idtr;

extern void* isr_stub_table[];

void idt_set_descriptor(uint16_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t) isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t) isr >> 16;
    descriptor->reserved = 0x08;
}

void default_interrupt_handler(void) {
    printf("default_interrupt_handler\n");
}

void idt_init(void) {
    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint16_t vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++) {
        void* isr = (vector < 32) ? isr_stub_table[vector] : default_interrupt_handler;
        idt_set_descriptor(vector, isr, 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}

void generic_exception_handler(void) {
    printf("OOPSIES\n");
    __asm__ volatile ("cli; hlt");
}
