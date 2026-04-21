#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>
#include "array.h"
#include "cursor.h"

typedef struct{
    const char *filename;
    Array lines;
    Cursor cursor;
    bool dirty;
}Buffer;

Buffer *create_buffer(const char *filename);
void load_file(Buffer *buffer);
void destroy_buffer(Buffer *buffer);

#endif

