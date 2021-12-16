#include "../include/bmp.h"
#include "../include/rotate.h"
#include "../include/util.h"

const uint8_t ARG_COUNT = 3;

void usage() {
    fprintf(stderr, "Usage: ./image-transformer <source-image> <transformed-image>\n"); 
}

image read_file(const char* filename) {
    FILE *in = NULL;
    image img = {0};

    open_status o_status = open_file(filename, &in, "rb");
    if (o_status != OPEN_OK) err("open input file %s failed with code: %d", filename, o_status);
    
    read_status r_status = from_bmp(in, &img);
    if (r_status != READ_OK) {
        close_file(in);
        err("read input file %s failed with code: %d", filename, r_status);
    }

    close_status c_status = close_file(in);
    if (c_status != CLOSE_OK) err("close input file %s failed with code: %d", filename, c_status);
    return img;
}

void write_file(const char* filename, const image img) {
    FILE *out = NULL;

    open_status o_status = open_file(filename, &out, "wb");
    if (o_status != OPEN_OK) err("open output file %s failed with code: %d", filename, o_status);
    
    write_status w_status = to_bmp(out, &img);
    if (w_status != WRITE_OK) {
        close_file(out);
        err("write to output file %s failed with code: %d", filename, w_status);
    }
    
    close_status c_status = close_file(out);
    if (c_status != CLOSE_OK) err("close output file %s failed with code: %d", filename, c_status);
}

int main( int argc, char** argv ) {
    if (argc != ARG_COUNT) usage();
    if (argc < ARG_COUNT) err("Not enough arguments \n" );
    if (argc > ARG_COUNT) err("Too many arguments \n" );
    

    image img = read_file(argv[1]);
    image rotated = rotate(img);
    write_file(argv[2], rotated);
    
    image_clear(&img);
    image_clear(&rotated);

    return 0;
}
