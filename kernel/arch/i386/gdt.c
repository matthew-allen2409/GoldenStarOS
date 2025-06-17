#include "kernel/gdt.h"

#define GDT_ENTRIES 3

extern void load_gdt(uint32_t);

__attribute__((aligned(8)))
gdt_entry_t gdt[GDT_ENTRIES];
gdt_ptr_t gdt_ptr;

void set_gdt_entry(int index,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t flags) {

    gdt_entry_t* entry = &gdt[index];

    entry->base_low = base & 0xFFFF;
    entry->base_mid = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->limit_low = limit & 0xFFFF;

    entry->access = access;

    entry->flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void init_gdt() {
  gdt_ptr.limit = sizeof(gdt) - 1;
  gdt_ptr.base = (uint32_t) &gdt;

  set_gdt_entry(0, 0, 0, 0, 0);
  set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xC0);
  set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xC0);

  load_gdt((uint32_t) &gdt_ptr);
}
