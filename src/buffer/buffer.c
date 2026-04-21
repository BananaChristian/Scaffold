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

void buffer_insert_char(Buffer *buffer,char ch){
    char *old_line=array_get(&buffer->lines,buffer->cursor.pos_y);
    size_t old_len=strlen(old_line);
    
    size_t pos_x=buffer->cursor.pos_x;
    char *new_line=malloc(old_len+2);
    memcpy(new_line,old_line,pos_x);
    new_line[pos_x]=ch;
    memcpy(new_line+pos_x+1,old_line+pos_x,old_len-pos_x+1);
    
    array_set(&buffer->lines,buffer->cursor.pos_y,(char *)new_line);
    free(old_line);
    buffer->cursor.pos_x++;
    buffer->dirty=true;
}

void buffer_delete_char(Buffer *buffer){
    char *old_line=array_get(&buffer->lines,buffer->cursor.pos_y);
    size_t old_len=strlen(old_line);
    char *new_line=malloc(old_len);
    size_t pos_x=buffer->cursor.pos_x;
    memcpy(new_line,old_line,pos_x);
    memcpy(new_line+pos_x,old_line+pos_x+1,old_len-pos_x);
    
    array_set(&buffer->lines,buffer->cursor.pos_y,new_line);
    free(old_line);
    
    buffer->cursor.pos_y--;
    buffer->dirty=true;
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

void buffer_move_cursor_up(Buffer *buffer){
    if(buffer->cursor.pos_y==0){
        buffer->cursor.pos_y=0;
        return;
    }
    
    buffer->cursor.pos_y--;
}

void buffer_move_cursor_down(Buffer *buffer){
    buffer->cursor.pos_y++;
}

void buffer_move_cursor_left(Buffer *buffer){
    if(buffer->cursor.pos_x==0){
        buffer_move_cursor_up(buffer);
        //I need to set the x position to the end of the last line
        return;
    }
    buffer->cursor.pos_x--;
}

void buffer_move_cursor_right(Buffer *buffer){
    buffer->cursor.pos_x++;
}