#ifndef APIC_H
#define APIC_H

#include <stdbool.h>
#include <stdint.h>

#define IOAPIC_BASE_PHYS 0xFEC00000

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MMIO 0xFEE00000

#define LAPIC_SVR_OFFSET        0xF0
#define APIC_ENABLE      0x100
#define SPURIOUS_VECTOR  0xFF

bool cpu_has_apic(void);

void enable_apic(void);

void io_apic_init(void);

void lapic_write(uint32_t reg_offset, uint32_t value);

#endif

// steps to initialize the APIC
// 1. Check that the APIC is supported
// 2. Disable Legacy PIC
// 3. Enable APIC in the model specific register
// 4. Map the APIC mmio region to a known page - Ensure it is strong uncachable
// 5. Set spurious interrupt vector and enable set the APIC enabled flag.
