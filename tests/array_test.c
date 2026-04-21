#include <stdio.h>
#include "array.h"

int main() {
    Array array;
    int a=10,b=20,c=30;
    array_append(&array,&a);
    array_append(&array,&b);
    array_append(&array,&c);

    int *last = (int *)array_pop(&array);   // Pop c - works
    printf("%d\n", *last);                   // Prints 30
    
    int *second = (int *)array_pop(&array);  // Pop b - works
    printf("%d\n", *second);                 // Prints 20
    
    int *third = (int *)array_pop(&array);   // Pop a - works
    printf("%d\n", *third);                  // Prints 10
    
    array_delete(&array);
    
    return 0;
}