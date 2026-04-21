#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct{
    size_t size;
    size_t capacity;
    void **data;
    bool isInitialized;
}Array;

void array_init(Array *array);
size_t array_length(Array *array);
void *array_get(Array *array,size_t index);
bool array_isEmpty(Array *array);
void array_append(Array *array,void *value);
void *array_pop(Array *array);
void array_delete(Array *array);

#endif ARRAY_H
