#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "buffer.h"

typedef enum{
    NORMAL,
    INSERT,
    VISUAL,
}Mode;

typedef struct{
    Array open_buffers; //An array of all the open buffers
    Buffer *active_buffer; //The actual buffer we are concentrating on
    const char *working_directory; //The current directory the editor has been called in 
    bool needs_redraw;//A dirty flag to tell the renderer if it should redraw
                      
    char **cached_paths;//A list of all the files in the working directory
    size_t cache_count;
    const char *file_query; //The file the user asks for
    bool is_cache_valid;//Is the cached_paths valid
    
    Mode current_mode;//The current mode we are in 
}Workspace;

void ws_init(Workspace *ws);
void ws_refresh_files_cache(Workspace *ws);
void ws_free_cache(Workspace *ws);
void ws_create_buffer(Workspace *ws,const char *filename);
Buffer *ws_find_buffer(Workspace *ws,const char *filename);
const char *mode_to_text(Workspace *ws);
void ws_destroy(Workspace *ws);

#endif
