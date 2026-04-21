#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>
#include "raylib.h"
#include "workspace.h"

typedef enum{
    STATUS_BAR,
    BUFFER_AREA,
    FILEPICKER,
}Focus;

typedef struct{
    size_t pos_x,pos_y;
    size_t width,height;
    size_t font_size;
    Font font;
    Color bg_color;
}StatusBar;

typedef struct{
    size_t pos_x,pos_y;
    size_t width,height;
    size_t padding_top;
    size_t padding_left;
    size_t scroll_x,scroll_y;
    size_t font_size;
    Font font;
    Color bg_color;
}BufferArea;

typedef struct{
    size_t pos_x,pos_y;
    size_t width,height;
    size_t scroll_x,scroll_y;
    size_t padding_top,padding_bottom;
    size_t padding_left,padding_right;
    size_t font_size;
    size_t selected_index;
    //const char *file_query;
    Font font;
    Color bg_color;
    Color selection_color;
}FilePicker;

typedef struct{
    size_t pos_x,pos_y;
    size_t width,height;
    size_t font_size;
    Font font;
    Color bg_color;
}BottomBar;

//This holds the states of what is being rendered for example is the file picker on(for now that is only thing)
typedef struct{
    bool display_picker;
}UIstate;

typedef struct{
    //Overall dimensions
    size_t screen_width;
    size_t screen_height;

    //Components
    StatusBar status_bar;
    BufferArea buffer_area;
    FilePicker file_picker;
    BottomBar bottom_bar;
    
    //Focus
    Focus focus;

    //The state of the UI
    UIstate state;

    //These I will move to personality later
    Font ui_font;
    Font editor_font;
    Color bg_color;
    Color text_color;
    Color cursor_color;

    //Cursor state
    double cursor_blink_timer;
    bool cursor_visible;
}Renderer;

void render_init(Renderer *render);
void render_status_bar(Renderer *render,Workspace *ws);
void render_buffer_area(Renderer *render,Workspace *ws);
void render_filepicker(Renderer *render,Workspace *ws);
void render_bottom_bar(Renderer *render,Workspace *ws);
void render_calculate_layout(Renderer *render);
void process_keys(Renderer *render,Workspace *ws);
void render_draw(Renderer *render,Workspace *ws);

#endif
