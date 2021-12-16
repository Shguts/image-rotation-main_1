#include "../include/image.h"
#include <stdlib.h>

void image_init(image *img, uint64_t width, uint64_t height) {
    img->width = width;
    img->height = height;
    img->data = (pixel *) malloc(sizeof(pixel) * width * height);
}

void image_clear(image *img) {
    free(img->data);
}
