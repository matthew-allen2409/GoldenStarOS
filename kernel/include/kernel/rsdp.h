#ifndef RSDP_H
#define RSDP_H

#include <stdint.h>

typedef struct {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdt_addr;
} __attribute__((packed)) rsdp_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMTable_id[8];
    uint32_t OEM_revision;
    uint32_t creator_id;
    uint32_t creator_revsion;
} __attribute__((packed)) rsdt_header_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMTable_id[8];
    uint32_t OEM_revision;
    uint32_t creator_id;
    uint32_t creator_revsion;
    uint32_t local_interrupt_controller_addr;
    uint32_t flags;
} __attribute__((packed)) madt_t;

typedef struct {
    rsdt_header_t header;
    uint32_t ptrs[];
} __attribute__((packed)) rsdt_t;

typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_addr;
    uint32_t global_system_interrupt_base;
} __attribute__((packed)) io_apic_t;

rsdp_t* locate_rsdp(void);

rsdt_t* get_rsdt(void);

madt_t* get_madt(void);

#endif
