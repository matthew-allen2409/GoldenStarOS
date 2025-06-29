#include <kernel/rsdp.h>
#include <kernel/memory.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t checksum(uint8_t* data, uint32_t length) {
    uint8_t sum = 0;

    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }

    return sum;
}

bool valid_rsdp(uint8_t* ptr) {
    return (memcmp(ptr, "RSD PTR ", 8) == 0) && (checksum(ptr, 20) == 0);
}


rsdp_t* locate_rsdp(void) {
    uint16_t ebda_seg = *(uint16_t*)0x40E;
    uint32_t ebda_addr = ((uint32_t)ebda_seg) << 4;

    for (uint8_t* ptr = (uint8_t*) ebda_addr; ptr < (uint8_t*)(ebda_addr + 1024); ptr += 16) {
        if (valid_rsdp(ptr)) {
            return (rsdp_t*) ptr;
        }
    }

    for (uint8_t* ptr = (uint8_t*)0x000E0000; ptr < (uint8_t*)0x000FFFFF; ptr += 16) {
        if (valid_rsdp(ptr)) {
            return (rsdp_t*) ptr;
        }
    }

    return NULL;
}

rsdt_t* get_rsdt(void) {
    rsdp_t* rsdp = locate_rsdp();

    if (rsdp == NULL) {
        return NULL;
    }

    rsdt_t* rsdt = (rsdt_t*)rsdp->rsdt_addr;

    reserve_page((uintptr_t)rsdt);
    map_page((uintptr_t)rsdt, (uintptr_t)rsdt, PRESENT | READ_WRITE);

    uint32_t length = rsdt->header.length;

    reserve_region((uintptr_t)rsdt, length);
    map_region((uintptr_t)rsdt, (uintptr_t)rsdt, length, PRESENT | READ_WRITE);

    return rsdt;
}

madt_t* get_madt(void) {
    rsdt_t* rsdt = get_rsdt();

    // Need to make sure the entire MADT is reserved and mapped.
    size_t ptrs_len = (rsdt->header.length - sizeof(rsdt->header)) / 4;

    for (size_t i = 0; i < ptrs_len; i++) {
        rsdt_header_t* header = (rsdt_header_t*) rsdt->ptrs[i];

        reserve_page((uintptr_t)header);
        map_page((uintptr_t)header, (uintptr_t)header, PRESENT | READ_WRITE);
        uint32_t length = header->length;

        reserve_region((uintptr_t)header, length);
        map_region((uintptr_t)header, (uintptr_t)header, length, PRESENT | READ_WRITE);

        if (memcmp(header->signature, "APIC", 4) == 0) {
            return (madt_t*) header;
        }
    }

    return NULL;
}
