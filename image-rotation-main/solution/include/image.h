#pragma once
#include <stdint.h>

typedef struct pixel { 
  uint8_t b, g, r;
} pixel;

typedef struct image {
  uint64_t width, height;
  pixel* data;
} image;

void image_init(image *img, uint64_t width, uint64_t height);

void image_clear(image *img);
