#include<inttypes.h>


struct __attribute__((__packed__)) bmp_header{
    uint8_t p1;
    uint8_t p2;
    uint32_t size;
    uint16_t reserve1;
    uint16_t reserve2;
    uint32_t pixel_offset;
};