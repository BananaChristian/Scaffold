#ifndef CURSOR_H
#define CURSOR_H


#include <stddef.h>

typedef struct {
    size_t pos_x;
    size_t pos_y;
}Cursor;

void cursor_init(Cursor *cursor);
void cursor_up(Cursor *cursor);
void cursor_down(Cursor *cursor);

#endif
