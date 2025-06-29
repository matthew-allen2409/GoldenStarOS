#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint8_t checksum(uint8_t* data, uint32_t length) {
    uint8_t sum = 0;

    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }

    return sum;
}

#endif
