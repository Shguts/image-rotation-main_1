#include <stdint.h>
#include <stdio.h>
#include "image.h"
#include "filework.h"

typedef struct bmp_header
{
  uint16_t bfType;
  uint32_t bfileSize;
  uint32_t bfReserved;
  uint32_t bOffBits;
  uint32_t biSize;
  uint32_t biWidth;
  uint32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint32_t biXPelsPerMeter;
  uint32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} __attribute__((packed)) bmp_header;

read_status from_bmp( FILE* in, image* img );

write_status to_bmp( FILE* out, image const* img );
