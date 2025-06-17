#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <multiboot/multiboot.h>
#include <kernel/memory.h>
#include <kernel/gdt.h>

static uint32_t total_memory = 0;
static uint32_t total_pages = 0;
static uint8_t* page_bitmap;

static __attribute__((aligned(4096))) uint32_t page_directory[PAGE_DIRECTORY_SIZE];
static __attribute__((aligned(4096))) uint32_t first_page_table[PAGE_TABLE_SIZE];

static uint32_t kernel_heap_ptr = (uint32_t) &_endkernel;

bool check_bit(size_t bit) {
    size_t byte = bit / 8;

    return (page_bitmap[byte] & (1 << (bit % 8))) == 1;
}

void set_bit(size_t bit) {
    size_t byte = bit / 8;
    page_bitmap[byte] |= (1 << (bit % 8));
}

void clear_bit(size_t bit) {
    size_t byte = bit / 8;
    page_bitmap[byte] &= ~(1 << (bit % 8));
}

void init_identity_map() {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | READ_WRITE | PRESENT;
    }

    for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
        page_directory[i] = 0;
    }

    page_directory[0] = ((uint32_t) first_page_table & 0xFFFFF000) | READ_WRITE | PRESENT;
}

void* bump_alloc(uint32_t size, uint32_t alignment) {
    kernel_heap_ptr = (kernel_heap_ptr + alignment - 1) & ~(alignment - 1);

    void* result_addr = (void*) kernel_heap_ptr;
    kernel_heap_ptr += size;

    return result_addr;
}

void enable_paging() {
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void init_paging(uint32_t mmap_start, uint32_t mmap_length) {
    uint32_t mmap_end = mmap_start + mmap_length;

    for (uint32_t mmap_current = mmap_start; mmap_current < mmap_end;) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;
        uint32_t region_end = entry->addr_low + entry->length_low;

        if (region_end > total_memory) {
            total_memory = region_end;
        }

        mmap_current += entry->size + sizeof(entry->size);
    }

    uint32_t bitmap_size = total_memory / PAGE_SIZE / 8;
    page_bitmap = bump_alloc(bitmap_size, sizeof(uint8_t));
    memset(page_bitmap, 0xFF, bitmap_size);

    for (uint32_t mmap_current = mmap_start; mmap_current < mmap_end;) {
        mmap_entry_t* entry = (mmap_entry_t*) mmap_current;

        if (entry->type == 1) {
            size_t first_page = entry->addr_low / PAGE_SIZE;
            size_t last_page = (entry->addr_low + entry->length_low + PAGE_SIZE - 1) / PAGE_SIZE;

            for (size_t p = first_page; p < last_page; p++) {
                uint32_t addr = p * PAGE_SIZE;

                if (addr >= kernel_heap_ptr) {
                    clear_bit(p);
                }
            }
        }

        mmap_current += entry->size + sizeof(entry->size);
    }

    init_identity_map();

    page_directory[1023] = (uint32_t) page_directory | PRESENT | READ_WRITE;

    enable_paging();

    kernel_heap_ptr = HEAP_START;
}

void setup_memory(multiboot_info_t* mbinfo) {
    if (!CHECK_FLAG (mbinfo->flags, 6)) {
        printf("Invalid mmap flag! Failed to set up memory\n");
        return;
    }

    total_pages = total_memory / PAGE_SIZE;
    init_gdt();
    init_paging(mbinfo->mmap_addr, mbinfo->mmap_length);

    printf("Total memory: %u megabytes\n", total_memory / (1024 * 1024));
}

bool page_present(uintptr_t addr) {
    return check_bit(addr / PAGE_SIZE);
}

uintptr_t alloc_phys_page() {
    uint32_t num_pages = total_pages * 8;

    for (size_t p = 0; p < num_pages; p++) {
        if (!check_bit(p)) {
            set_bit(p);
            uintptr_t page_addr = p * PAGE_SIZE;
            memset((void*) page_addr, 0, PAGE_SIZE);
            return page_addr;
        }
    }

    return 0;
}

void map_page(uintptr_t virtual_addr, uintptr_t phys_addr, uint32_t flags) {
    size_t pd_idx = virtual_addr >> 22;
    size_t pt_idx = (virtual_addr >> 12) & 0x3FF;

    uint32_t pde = page_directory[pd_idx];

    if ((pde & PRESENT) == 0) {
        uintptr_t page = alloc_phys_page();
        memset((void*) page, 0, PAGE_SIZE);
        page_directory[pd_idx] = page | PRESENT | READ_WRITE | flags;
    }

    pde = page_directory[pd_idx];

    uint32_t* page_table = (uint32_t*) (0xFFC00000 + (pd_idx << 12));

    page_table[pt_idx] = phys_addr | flags | PRESENT;
}

void* kmalloc(uint32_t size, uint32_t alignment) {
    kernel_heap_ptr = (kernel_heap_ptr + alignment - 1) & ~(alignment - 1);

    uintptr_t base_addr = kernel_heap_ptr;
    kernel_heap_ptr += size;

    for (uintptr_t addr = base_addr; addr < kernel_heap_ptr; addr += PAGE_SIZE) {
        if (!page_present(addr)) {
            uintptr_t phys_addr = alloc_phys_page();
            map_page(addr, phys_addr, READ_WRITE);
        }
    }


    return (void*) base_addr;
}
