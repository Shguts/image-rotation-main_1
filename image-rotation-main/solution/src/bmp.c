#include "../include/bmp.h"
#include <stdlib.h>
#include <string.h>

const uint32_t BPF_FILE_TYPE = 0x4D42, BI_RGB = 0, IMAGE_PADDING = 4, BIT_MAP_INFO_SIZE = sizeof(bmp_header) - 14, BIT_PER_PIXEL = 24;

static read_status bmp_header_read( FILE* f, bmp_header* header ) {
    size_t ret_code = fread( header, sizeof( bmp_header ), 1, f );
    if (ret_code != 1) {
        if (feof(f))
            return READ_UNEXPECTED_EOF;
        return READ_INVALID_HEADER;
    }
    if (header->bfType != BPF_FILE_TYPE) {
        return READ_INVALID_SIGNATURE;
    } 
    return READ_OK;
}

static read_status pixel_read( FILE* f, pixel* p ) {
    size_t ret_code = fread( p, sizeof(pixel), 1, f );
    if (ret_code != 1) {
        if (feof(f)) return READ_UNEXPECTED_EOF;
        return READ_INVALID_BITS;
    }
    return READ_OK;
}

static void bmp_header_print(const bmp_header header) {
    printf("bfType: %d\nbfileSize: %d\nbfReserved: 0x%04X\nbOffBits: %d\nbiSize: %d\nbiWidth: %d\nbiHeight: %d\nbiPlanes: %d\nbiBitCount: %d\n\
biCompression: %d\nbiSizeImage: %d\nbiXPelsPerMeter: %d\nbiYPelsPerMeter: %d\nbiClrUsed: %d\nbiClrImportant: %d\n\n\n", 
        header.bfType, 
        header.bfileSize,
        header.bfReserved,
        header.bOffBits,
        header.biSize,
        header.biWidth,
        header.biHeight,
        header.biPlanes,
        header.biBitCount,
        header.biCompression,
        header.biSizeImage,
        header.biXPelsPerMeter,
        header.biYPelsPerMeter,
        header.biClrUsed,
        header.biClrImportant);
}

static uint32_t padding_length(uint64_t width) {
    return IMAGE_PADDING - ((width * sizeof(pixel)) % IMAGE_PADDING);
}

read_status from_bmp(FILE* in, image* img) {
    bmp_header header = {0};
    read_status status = bmp_header_read(in, &header);
    if (status != READ_OK) return status;
    printf("Read bmp header: \n");
    bmp_header_print(header);
    uint32_t padding = padding_length(header.biWidth);
    image_init(img, header.biWidth, header.biHeight);
    read_status r_status = READ_OK;
    for (uint64_t i = 0; i < header.biHeight; ++i) {
        for(uint64_t j = i * header.biWidth ; j < (i + 1) * header.biWidth; ++j) {
            r_status = pixel_read(in, &(img->data[j]));
            if (r_status != READ_OK) return r_status;
        }
        if (padding != 0) fseek(in, padding, SEEK_CUR);
    }
    return READ_OK;
}

static write_status bmp_header_write( FILE* f, bmp_header* header ) {
    size_t ret_code = fwrite( header, sizeof( bmp_header ), 1, f );
    if (ret_code != 1) {
        if (feof(f))
            return WRITE_UNEXPECTED_EOF;
        return ferror(f);
    } 
    return WRITE_OK;
}

static void bmp_create_header(image const* img, bmp_header* header) {
    uint32_t padding = padding_length(img->width);

    header->bfType = BPF_FILE_TYPE;
    header->bOffBits = sizeof(bmp_header);
    header->bfileSize = header->bOffBits + (sizeof(pixel) * img->height * img->width) + (img->height * padding);
    
    header->biSize = BIT_MAP_INFO_SIZE;
    header->biHeight = img->height;
    header->biWidth = img->width;
    header->biPlanes = 1;
    header->biBitCount = BIT_PER_PIXEL;
    header->biCompression = BI_RGB;
    header->biSizeImage = header->bfileSize - sizeof(bmp_header);
    header->biClrUsed = 0;
    header->biClrImportant = 0;
    header->biXPelsPerMeter = 0;
    header->biYPelsPerMeter = 0;
}

static write_status pixel_write( FILE* f, pixel* p ) {
    size_t ret_code = fwrite( p, sizeof(pixel), 1, f );
    if (ret_code != 1) {
        if (feof(f))
            return WRITE_UNEXPECTED_EOF;
        return ferror(f);
    } 
    return WRITE_OK;
}

static write_status padding_write(FILE *f, uint8_t* padding_bytes, uint32_t padding) {
    size_t ret_code = fwrite(padding_bytes, padding, sizeof(uint8_t), f);
    if (ret_code != 1) {
        if (feof(f))
            return WRITE_UNEXPECTED_EOF;
        return ferror(f);
    } 
    return WRITE_OK;
}

write_status to_bmp( FILE* out, image const* img ) {
    bmp_header header = {0};
    bmp_create_header(img, &header);
    write_status w_status = bmp_header_write(out, &header);
    if (w_status != WRITE_OK) return w_status;
    uint32_t padding = padding_length(header.biWidth);
    uint8_t* padding_bytes = NULL;
    if (padding != 0) {
        padding_bytes = (uint8_t *)malloc(sizeof(uint8_t) * padding);
        for (uint32_t i =  0 ; i < padding; ++i) padding_bytes[i] = 0;
    }
    for (uint64_t i = 0; i < header.biHeight; ++i) {
        for(uint64_t j = i * header.biWidth; j < (i + 1) * header.biWidth; ++j){
            w_status = pixel_write(out, &(img->data[j]));
            if (w_status != WRITE_OK) {
                if (padding != 0) free(padding_bytes);
                return w_status;
            }
        }
        if (padding != 0) {
            w_status = padding_write(out, padding_bytes, padding);
            if (w_status != WRITE_OK) {
                if (padding != 0) free(padding_bytes);
                return w_status;
            }
        }
    }
    if (padding != 0) free(padding_bytes);
    return WRITE_OK;
}
