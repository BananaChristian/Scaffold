#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "buffer.h"
#include "raylib.h"
#include "workspace.h"


void ws_init(Workspace *ws){
    const char *cwd=GetWorkingDirectory();
    ws->working_directory=malloc(strlen(cwd)+1); 
    strcpy((char*)ws->working_directory,cwd);
    ws->active_buffer=NULL;
    ws->needs_redraw=true;

    ws->current_mode=NORMAL;
    ws->cached_paths=NULL;
    ws->cache_count=0;
    ws->file_query=NULL;
    ws->is_cache_valid=false;

    array_init(&ws->open_buffers);
    ws_refresh_files_cache(ws);
}

void ws_free_cache(Workspace *ws){
    if(!ws->is_cache_valid)
        return;

    for(size_t i=0;i<ws->cache_count;i++){
        free(ws->cached_paths[i]);
    }
    free(ws->cached_paths);   
    ws->cache_count=0;
    ws->cached_paths=NULL;
    ws->is_cache_valid=false;
}

void ws_refresh_files_cache(Workspace *ws){
    ws_free_cache(ws);    
    
    //Load from raylib
    FilePathList files=LoadDirectoryFilesEx(ws->working_directory,NULL,true);
    ws->cache_count=files.count;
    ws->cached_paths=malloc(files.count*sizeof(char *));
    for(size_t i=0;i<files.count;i++){
        ws->cached_paths[i]=malloc(strlen(files.paths[i])+1);
        strcpy(ws->cached_paths[i],files.paths[i]);
    }

    UnloadDirectoryFiles(files);
    ws->is_cache_valid=true;
}

Buffer *ws_find_buffer(Workspace *ws,const char *filename){
    for(size_t i=0;i<ws->open_buffers.size;i++){
        Buffer *buffer=array_get(&ws->open_buffers, i);
        if(strcmp(buffer->filename,filename)==0){
            return buffer;
        }
    }
    return NULL;
}

void ws_create_buffer(Workspace *ws,const char *filename){
    //First check if the buffer exists and if it does return that
    Buffer *existing_buffer=ws_find_buffer(ws,filename);
    if(existing_buffer){
       ws->active_buffer=existing_buffer; 
       return;
    }
    
    //If the buffer doesnt exist go ahead and create a new one
    Buffer *new_buffer=create_buffer(filename);
    array_append(&ws->open_buffers, new_buffer);
    ws->active_buffer=new_buffer;
}

void ws_destroy(Workspace *ws){
    free((void*)ws->working_directory);
    ws->working_directory=NULL;
    if(ws->active_buffer)
        destroy_buffer(ws->active_buffer);
    
    ws->active_buffer=NULL;
    ws->needs_redraw=false;

    ws_free_cache(ws);
    array_delete(&ws->open_buffers);
}

const char *mode_to_text(Workspace *ws){
    char *mode_name=malloc(sizeof(char*));
    switch(ws->current_mode){
        case NORMAL:{
            mode_name="NORMAL";
            return mode_name;
        }  
        case VISUAL:{
            mode_name="VISUAL";
            return mode_name;
        }
        case INSERT:{
            mode_name="INSERT";
            return mode_name;
        }
        default:
            mode_name="";
    }
    
    return mode_name;
}
