#include <stdlib.h>
#include <assert.h>
#include "array.h"

void array_init(Array *array){   
   array->size=0;
   array->capacity=8;
   array->data=malloc(array->capacity*sizeof(void*));
   assert(array->data != NULL);
   array->isInitialized=true;
}

size_t array_length(Array *array){
    return array->size;
}

bool array_isEmpty(Array *array){
    if(array->size==0)
        return true;

    return false;
}

void *array_get(Array *array,size_t index){
    if(index >= array->size){
        return NULL;
    }
    return array->data[index];
}

void array_set(Array *array,size_t index,void *value){
    if(index>=array->size)
        return;
    
    array->data[index]=value;
}

void array_append(Array *array,void *value){
    if(!array->isInitialized)
        array_init(array);
    
    //If the array size gets bigger than capacity make it grow
    if(array->size == array->capacity){
        size_t new_capacity=array->capacity*2;
        void *temp=realloc(array->data,new_capacity*sizeof(void*));
        assert(temp != NULL);
        array->data=temp;
        array->capacity=new_capacity;
    }

    array->data[array->size]=value;
    array->size++;
}

void *array_pop(Array *array){
    if(array->size == 0)
        return NULL;

    void *last_element=array->data[array->size-1];
    array->size--;
    return last_element;
}

void array_delete(Array *array){
   if(!array->isInitialized)
       return;
   
   free(array->data);
   array->capacity=0;
   array->size=0;
   array->data=NULL;
   array->isInitialized=false;
}
