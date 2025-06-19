#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
    uint16_t    isr_low;
	uint16_t    kernel_cs;
	uint8_t     reserved;
	uint8_t     attributes;
	uint16_t    isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);

#endif
