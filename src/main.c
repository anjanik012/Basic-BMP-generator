#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "bmp_header.h"
#include "dib_header.h"
#include "pixel.h"

#define BITS_PER_PIXEL 24


uint32_t row_size(int32_t width) {
    int32_t pad_size = width * (BITS_PER_PIXEL / 8) % 4;
    int32_t r_size = pad_size + width * (BITS_PER_PIXEL / 8);
    return r_size;
}


uint32_t pixel_array_size(int32_t width, int32_t height) {
    return height * row_size(width);
}


void init_header(struct bmp_header *header, uint32_t image_size) {
    header->p1 = 0x42;
    header->p2 = 0x4d;
    header->size = image_size + sizeof(struct bmp_header) + sizeof(struct dib_header);
    header->reserve1 = 0x00;
    header->reserve2 = 0x00;
    header->pixel_offset = 0x36;
}


void init_dib_header(struct dib_header *dib_head, int32_t width, int32_t height) {
    dib_head->size = 0x28;
    dib_head->bitmap_width = width;
    dib_head->bitmap_height = height;
    dib_head->color_panes = 0x01;
    dib_head->bits_per_pixel = BITS_PER_PIXEL;
    dib_head->compression = 0x00;
    dib_head->horizontal_res = 0x00;
    dib_head->vertical_res = 0x00;
    dib_head->color_pallete = 0x00;
    dib_head->important_colors = 0x00;

    // set pixel array size
    dib_head->image_size = pixel_array_size(width, height);
}


int main(int argc, char *argv[]) {
    int opt;
    uint8_t red, blue, green;
    int32_t width, height;
    char *err;
    errno = 0;
    long val;
    char file_name[250];

    while((opt = getopt(argc, argv, "r:b:g:w:h:")) != -1) {
        switch(opt) {
            case 'r':
                val = strtol(optarg, &err, 10);
                if (err == optarg || *err != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE) || (val < 0 || val > 255)) {
                    printf("Invalid value given for red pixelt\n");
                    return 1;
                }
                red = val;
                break;
            case 'b':
                val = strtol(optarg, &err, 10);
                if (err == optarg || *err != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE) || (val < 0 || val > 255)) {
                    printf("Invalid value given for blue pixelt\n");
                    return 1;
                }
                blue = val;
                break;
            case 'g':
                val = strtol(optarg, &err, 10);
                if (err == optarg || *err != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE) || (val < 0 || val > 255)) {
                    printf("Invalid value given for green pixelt\n");
                    return 1;
                }
                green = val;
                break;
            case 'w':
                val = strtol(optarg, &err, 10);
                if (err == optarg || *err != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE)) {
                    printf("Invalid value given image widtt\n");
                    return 1;
                }
                width = val;
                break;
            case 'h':
                val = strtol(optarg, &err, 10);
                if (err == optarg || *err != '\0' || ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE)) {
                    printf("Invalid value given for image height\n");
                    return 1;
                }
                height = val;
                break;
                    
        }
        
    }

    if (optind < argc) {
        strcpy(file_name, argv[optind]);
    } else {
        printf("No file name given to save the file\n");
        return 1;
    }

    printf("Saving image to file name: %s\n", file_name);
    printf("Using color for each pixel, red: %d, blue: %d, green: %d\n", red, blue, green);
    printf("Image resolution: %dX%d\n", width, height);

    struct bmp_header header;
    uint32_t px_arr_size = pixel_array_size(width, height);
    init_header(&header, px_arr_size);

    struct dib_header dib_head;
    init_dib_header(&dib_head, width, height);

    int32_t r_size = row_size(width);

    void *rows = malloc(px_arr_size);
    memset(rows, 0, px_arr_size);

    uint32_t index = 0;
    for (int i = 0; i < height; i++) {
        struct pixel *pxs = malloc(width * sizeof(struct pixel));
        for (int j = 0; j < width; j++) {
            struct pixel px = {blue, green, red};
            pxs[j] = px;
        }
        memcpy(rows + index, pxs, r_size);
        index += r_size;
        free(pxs);
    }

    FILE *fp;
    fp = fopen(file_name, "wb");
    if (fp == NULL) {
        printf("Unable to open file %s\n", file_name);
        return 1;
    }

    int32_t write_result = 0;
    uint64_t bytes_written = 0;
    write_result = fwrite(&header, sizeof(struct bmp_header), 1, fp);
    if (write_result == 1) {
        bytes_written += sizeof(struct bmp_header);
    } else {
        printf("Failed to write BMP header\n");
        return 1;
    }
        

    write_result = fwrite(&dib_head, sizeof(struct dib_header), 1, fp);
    if (write_result == 1) {
        bytes_written += sizeof(struct dib_header);
    } else {
        printf("Failed to write DIB header\n");
        return 1;
    }

    write_result = fwrite(rows, r_size, height, fp);
    if (write_result == height) {
        bytes_written += r_size * height;
    } else {
        printf("Failed to write Pixel array data\n");
        return 1;
    }

    fclose(fp);
    free(rows);

    printf("Bytes written: %d to file: %s\n", bytes_written, file_name);
    
    return 0;
}
