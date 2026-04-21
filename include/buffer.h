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
void buffer_insert_char(Buffer *buffer,char ch);
void buffer_delete_char(Buffer *buffer);
void destroy_buffer(Buffer *buffer);

void buffer_move_cursor_left(Buffer *buffer);
void buffer_move_cursor_right(Buffer *buffer);
void buffer_move_cursor_down(Buffer *buffer);
void buffer_move_cursor_up(Buffer *buffer);

#endif

