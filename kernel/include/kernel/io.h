#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline uint64_t rdmsr(uint32_t msr_id) {
    uint64_t msr_value;
    asm volatile ( "rdmsr" : "=A" (msr_value) : "c" (msr_id) );
    return msr_value;
}

static inline void wrmsr(uint32_t msr_id, uint64_t msr_value) {
    asm volatile ( "wrmsr" : : "c" (msr_id), "A" (msr_value) );
}

#endif
