#define _POSIX_C_SOURCE 200809L
#include "buffer.h"
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

Buffer *create_buffer(const char *filename){
    Buffer *new_buffer=malloc(sizeof(Buffer));
    new_buffer->cursor.pos_x=0;
    new_buffer->cursor.pos_y=0;
    new_buffer->filename=malloc(strlen(filename)+1);
    strcpy((char *)new_buffer->filename,filename);
    new_buffer->dirty=true;

    load_file(new_buffer);

    return new_buffer;
}

void load_file(Buffer *buffer){
    FILE *file=fopen(buffer->filename,"r");
    if(file==NULL){
        file=fopen(buffer->filename,"w");
        perror("Failed to create file");
    }
    char *line=NULL;
    size_t len=0;
    ssize_t read;

    while((read=getline(&line,&len,file))!=-1){
        if(read>0 && line[read-1]=='\n'){
            line[read-1]='\0';
        }
        array_append(&buffer->lines,strdup(line));
    }
    free(line);
    fclose(file);
}

void destroy_buffer(Buffer *buffer){
    buffer->cursor.pos_x=0;
    buffer->cursor.pos_y=0;
    buffer->dirty=false;

    free((void*)buffer->filename);
    buffer->filename=NULL;
    array_delete(&buffer->lines);
    free(buffer);
}
