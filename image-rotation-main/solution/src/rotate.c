#include "../include/rotate.h"
#include <stdio.h> 

/*
data: 1 2 3 4 5 6 7 8 9 a b c

1 2 3
4 5 6
7 8 9
a b c

rotate =>  

a 7 4 1
b 8 5 2
c 9 6 3

write to file =>

3 6 9 c
2 5 8 b
1 4 7 a
*/


image rotate( image const source ) {
    image rotated = {0};
    image_init(&rotated, source.height, source.width);
    for (uint64_t i = 0, k = 0; i < source.width; ++i) {
        for (uint64_t j = source.height; j > 0 ; --j) {
            rotated.data[k++] = source.data[(j - 1) * source.width + i];
        }
    }
    return rotated;
}
