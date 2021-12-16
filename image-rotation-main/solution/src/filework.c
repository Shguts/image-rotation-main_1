#include "../include/filework.h"

open_status open_file(const char *filename, FILE** f, const char *mode) {
    if (!filename) 
        return OPEN_ERROR;
    *f = fopen( filename, mode ); 
    if (!*f) 
        return OPEN_ERROR;
    return OPEN_OK;
}

close_status close_file(FILE* f) {
    return fclose(f);
}
