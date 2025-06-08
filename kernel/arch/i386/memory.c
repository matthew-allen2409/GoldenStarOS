#include <stdio.h>
#include <string.h>

#include <multiboot/multiboot.h>
#include <kernel/memory.h>

uint8_t page_bitmap[BITMAP_SIZE];

uint32_t kernel_start_phys = (uint32_t)&_kernel_start;
uint32_t kernel_end_phys = (uint32_t)&_endkernel;

void setup_memory(multiboot_info_t* mbinfo) {
    if (!CHECK_FLAG (mbinfo->flags, 6)) {
        printf("Invalid mmap flag! Failed to set up memory\n");
        return;
    }

    uint32_t max_addr = 0;

    uint32_t mmap_end = mbinfo->mmap_addr + mbinfo->mmap_length;
    uint32_t mmap_current = mbinfo->mmap_addr;

    while(mmap_current < mmap_end) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;

        uint32_t region_end = entry->addr_low + entry->length_low;
        if (region_end > max_addr) {
            max_addr = region_end;
        }

        printf("entry addr: 0x%x | entry length: %u bytes | entry type: %u\n",
            entry->addr_low,
            entry->length_low,
            entry->type);

        mmap_current += entry->size + sizeof(entry->size);
    }

    printf("Total memory: %u megabytes\n", max_addr / (1024 * 1024));

    uint32_t total_pages = max_addr / PAGE_SIZE;
    uint32_t total_bitmap_size_bytes = (total_pages + 7) / 8;

    memset(page_bitmap, 0xFF, total_bitmap_size_bytes);

    mmap_current = mbinfo->mmap_addr;
    while(mmap_current < mmap_end) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;

        if (entry->type == 1) {
          size_t first_page = entry->addr_low / PAGE_SIZE;
          size_t last_page = (entry->addr_low + entry->length_low) / PAGE_SIZE;

          for(size_t p = first_page; p < last_page; p++) {
              uint32_t addr = p * PAGE_SIZE;

              if (addr >= kernel_start_phys && addr < kernel_end_phys) {
                  continue;
              }

              page_bitmap[p / 8] &= ~(1 << (p % 8));
          }
        }

        mmap_current += entry->size + sizeof(entry->size);
    }

    printf("YAY MEMORY!\n");
}

