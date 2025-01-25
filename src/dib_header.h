#include<inttypes.h>


struct __attribute__((__packed__)) dib_header{
    // BITMAPINFOHEADER 40 bytes
    uint32_t size;
    int32_t bitmap_width;
    int32_t bitmap_height;
    uint16_t color_panes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t color_pallete;
    uint32_t important_colors;
};