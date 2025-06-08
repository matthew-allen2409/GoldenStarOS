#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <multiboot/multiboot.h>
#include <kernel/memory.h>

uint8_t page_bitmap[BITMAP_SIZE];

uint32_t kernel_start_phys = (uint32_t)&_kernel_start;
uint32_t kernel_end_phys = (uint32_t)&_endkernel;

uint32_t init_paging(uint32_t mmap_current, uint32_t mmap_length) {
    memset(page_bitmap, 0xFF, BITMAP_SIZE);

    uint32_t highest_addr = 0;

    uint32_t mmap_end = mmap_current + mmap_length;
    while (mmap_current < mmap_end) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;

        uint32_t region_end = entry->addr_low + entry->length_low;

        if (region_end > highest_addr) {
            highest_addr = region_end;
        }

        if (entry->type == 1) {
          size_t first_page = entry->addr_low / PAGE_SIZE;
          size_t last_page = (entry->addr_low + entry->length_low) / PAGE_SIZE;

          for (size_t p = first_page; p < last_page; p++) {
              uint32_t addr = p * PAGE_SIZE;

              if (addr >= kernel_start_phys && addr < kernel_end_phys) {
                  continue;
              }

              page_bitmap[p / 8] &= ~(1 << (p % 8));
          }
        }

        mmap_current += entry->size + sizeof(entry->size);
    }

    return highest_addr;
}

void setup_memory(multiboot_info_t* mbinfo) {
    if (!CHECK_FLAG (mbinfo->flags, 6)) {
        printf("Invalid mmap flag! Failed to set up memory\n");
        return;
    }

    uint32_t max_addr = init_paging(mbinfo->mmap_addr, mbinfo->mmap_length);

    printf("Total memory: %u megabytes\n", max_addr / (1024 * 1024));
}

bool check_bit(size_t bit) {
    size_t byte = bit / 8;
    
    return (page_bitmap[byte] & (1 << (bit % 8))) == 0;
}

void set_bit(size_t bit) {
    size_t byte = bit / 8;
    page_bitmap[byte] |= (1 << (bit % 8));
}

uint32_t alloc_page() {
    uint32_t num_pages = BITMAP_SIZE * 8;

    for (size_t p = 0; p < num_pages; p++) {
        if (check_bit(p)) {
            set_bit(p);
            return p * PAGE_SIZE;
        }
    }

    return 0;
}
