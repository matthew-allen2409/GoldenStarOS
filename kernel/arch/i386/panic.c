#include "kernel/panic.h"
#include <stdio.h>
#include <stdarg.h>

void panic(const char* fmt, ...) {
    asm volatile ("cli");

    va_list ap;
    va_start(ap, fmt);

    printf("KERNEL PANIC!\n");
    printf(fmt, ap);

    for (;;) asm volatile ("hlt");
}
