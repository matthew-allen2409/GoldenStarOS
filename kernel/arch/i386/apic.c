#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <kernel/apic.h>
#include <kernel/memory.h>
#include <kernel/io.h>
#include <kernel/rsdp.h>

static volatile uint32_t* ioapic_base = (volatile uint32_t*) IOAPIC_BASE_PHYS;

bool cpu_has_apic(void) {
    uint32_t eax, ebx, ecx, edx;
    __asm__ volatile ("cpuid"
                      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                      : "a"(1));
    return edx & (1 << 9);
}

void lapic_write(uint32_t reg_offset, uint32_t value) {
    volatile uint32_t* lapic = (volatile uint32_t*) IA32_APIC_BASE_MMIO;
    lapic[reg_offset / 4] = value;
}

uintptr_t get_io_apic_mmap() {
    madt_t* madt = get_madt();

    if (madt == NULL) {
        printf("madt is NULL\n");
        return 0;
    }

    uint8_t* entries_ptr = (uint8_t*) madt + sizeof(madt_t);
    uint8_t* entries_end = (uint8_t*) madt + madt->length;

    while (entries_ptr < entries_end) {
        uint8_t type = entries_ptr[0];
        uint8_t length = entries_ptr[1];

        if (length < 2 || entries_ptr + length > entries_end) break;

        if (type == 1) {
            io_apic_t* io_apic_struct = (io_apic_t*) entries_ptr;
            uintptr_t io_apic_addr = io_apic_struct->io_apic_addr;
            reserve_page(io_apic_addr);
            map_page(io_apic_addr, io_apic_addr, PRESENT | READ_WRITE | WRITE_THROUGH);
            return io_apic_addr;
        }

        entries_ptr += length;
    }

    return 0;
}

void io_apic_init(void) {
    madt_t* madt = get_madt();
    printf("madt addr: 0x%x\n", madt);

    if (madt == NULL) {
        // TODO: Need to add a panic here
        printf("BIG PROBLEM\n");
    }

    uint32_t apic_base = madt->local_interrupt_controller_addr;
    uint32_t IO_apic_base = get_io_apic_mmap();

    printf("local_apic addr: 0x%x\n", apic_base);
    printf("io_apic addr: 0x%x\n", IO_apic_base);

    reserve_page((uintptr_t) apic_base);
    map_page(apic_base,
            apic_base,
            PRESENT | READ_WRITE | WRITE_THROUGH | CACHE_DISABLE);
}

void enable_apic(void) {
    reserve_page((uintptr_t) IA32_APIC_BASE_MMIO);
    map_page(IA32_APIC_BASE_MMIO,
            IA32_APIC_BASE_MMIO,
            PRESENT | READ_WRITE | WRITE_THROUGH | CACHE_DISABLE);

    wrmsr(IA32_APIC_BASE_MSR, IA32_APIC_BASE_MMIO | (1 << 11));

    lapic_write(LAPIC_SVR_OFFSET, SPURIOUS_VECTOR | APIC_ENABLE);
}
