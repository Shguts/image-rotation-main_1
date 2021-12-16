#include <stdbool.h>
#include <stdio.h>

/*  deserializer   */
typedef enum read_status  {
  READ_OK = 0,
  READ_INVALID_SIGNATURE,
  READ_INVALID_BITS,
  READ_INVALID_HEADER,
  READ_UNEXPECTED_EOF
  /* коды других ошибок  */
} read_status;

/*  serializer   */
typedef enum write_status  {
  WRITE_OK = 0,
  WRITE_ERROR,
  WRITE_UNEXPECTED_EOF
  /* коды других ошибок  */
} write_status;

typedef enum open_status {
    OPEN_OK = 0,
    OPEN_ERROR
} open_status;

typedef enum close_status {
    CLOSE_OK = 0,
    CLOSE_ERROR,
} close_status;

open_status open_file(const char *filename, FILE** f, const char *mode);

close_status close_file(FILE* f);
