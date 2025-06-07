#include <stdio.h>

#include <multiboot/multiboot.h>
#include <kernel/memory.h>

uint8_t page_bitmap[BITMAP_SIZE];

void setup_memory(multiboot_info_t* mbinfo) {
    if (!CHECK_FLAG (mbinfo->flags, 6)) {
        printf("Invalid mmap flag! Failed to set up memory\n");
        return;
    }

    uint32_t mmap_end = mbinfo->mmap_addr + mbinfo->mmap_length;
    uint32_t mmap_current = mbinfo->mmap_addr;

    while(mmap_current < mmap_end) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;

        printf("entry addr: 0x%x\nentry length: 0x%x\nentry type: 0x%x\n",
            entry->addr_low,
            entry->length_low,
            entry->type);

        mmap_current += entry->size + sizeof(entry->size);
    }

    printf("YAY MEMORY!\n");
}
