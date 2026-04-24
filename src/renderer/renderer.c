#include "renderer.h"
#include "buffer.h"
#include "raylib.h"
#include "workspace.h"
#include <stdlib.h>
#include <string.h>

void render_init(Renderer *render){
    render->focus=BUFFER_AREA;
    render->state.display_picker=false;
    
    render->ui_font=LoadFont("assets/fonts/InterVariable.ttf"/*, 24, 0, 250 */);
    render->editor_font=LoadFont("assets/fonts/JetBrainsMono-Regular.ttf"/*,32,0,250 */);
    
    render->bottom_bar.font=render->ui_font;
    render->file_picker.font=render->ui_font;
    render->status_bar.font=render->ui_font;
    render->buffer_area.font=render->editor_font;

    render->bg_color=DARKGRAY;
    render->text_color=WHITE;
    render->cursor_color=BLUE;
    render->status_bar.bg_color=BLUE;
    render->file_picker.bg_color=BLUE;
    render->bottom_bar.bg_color=BLUE;
    render->file_picker.selection_color=LIGHTGRAY;

    render->cursor_blink_timer=0.0;
    render->cursor_visible=true;
    
    render_calculate_layout(render);
}

void render_calculate_layout(Renderer *render){
    render->screen_width=GetScreenWidth();
    render->screen_height=GetScreenHeight();
    
    //The status bar component
    render->status_bar.pos_x=0;
    render->status_bar.pos_y=0;
    render->status_bar.width=render->screen_width;
    render->status_bar.height=render->screen_height*0.05;
    render->status_bar.font_size=render->status_bar.height*0.5;
    
    //The buffer component
    render->buffer_area.pos_x=0;
    render->buffer_area.pos_y=render->status_bar.height;
    render->buffer_area.width=render->screen_width;
    render->buffer_area.height=render->screen_height*0.90;    
    render->buffer_area.padding_left=20;
    render->buffer_area.bg_color=render->bg_color;
    render->buffer_area.font_size= 18;//render->screen_height*0.02;
    render->buffer_area.scroll_x=0;
    render->buffer_area.scroll_y=0;
    
    //The file picker component
    render->file_picker.width=render->screen_width*0.4;
    render->file_picker.height=render->screen_height*0.5;
    render->file_picker.pos_x = (render->screen_width - render->file_picker.width) / 2;
    render->file_picker.pos_y = (render->screen_height - render->file_picker.height) / 2;
    render->file_picker.scroll_x=0;
    render->file_picker.scroll_y=0;
    render->file_picker.font_size = render->screen_height * 0.018; 
    render->file_picker.padding_left = 15;
    render->file_picker.padding_top = 10;
    render->file_picker.padding_right=5;
    render->file_picker.padding_bottom=5;
    render->file_picker.selected_index=0;
    
    //Bottom bar
    render->bottom_bar.width=render->screen_width;
    render->bottom_bar.height=render->screen_height*0.05;
    render->bottom_bar.font_size=render->bottom_bar.height*0.5;
    render->bottom_bar.pos_x=0;
    render->bottom_bar.pos_y=render->screen_height-render->bottom_bar.height;
}

void render_draw(Renderer *render,Workspace *ws){
    update_cursor_blink(render,GetFrameTime());
    process_keys(render,ws);
    BeginDrawing();
    ClearBackground(render->bg_color);
    render_status_bar(render,ws);
    render_buffer_area(render,ws);
    render_bottom_bar(render, ws);
    
    if(render->state.display_picker)
        render_filepicker(render,ws);

    EndDrawing();
}

void render_buffer_area(Renderer *render, Workspace *ws) {
    size_t pos_x=render->buffer_area.pos_x;
    size_t pos_y=render->buffer_area.pos_y;

    size_t width=render->buffer_area.width;
    size_t height=render->buffer_area.height;
    
    size_t font_size=render->buffer_area.font_size;
    
    DrawRectangle(pos_x,pos_y,width,height,render->buffer_area.bg_color); 
    
    Buffer *buf = ws->active_buffer;
    if (!buf){
        size_t center_x=render->buffer_area.width/2;
        size_t center_y=render->buffer_area.height/2;
        
        DrawText("SCAFFOLD",center_x, center_y, 40, WHITE);
        return;
    }

    render_cursor(render,ws,buf);
    
    size_t start_y = render->status_bar.height;  // Starting Y position (below status bar)
    size_t line_height = 20;  // Space between lines
    size_t x = 20;  // Left margin
    Vector2 pos={x,0};
    
    for (size_t i = 0; i < buf->lines.size; i++) {
        char *line = array_get(&buf->lines, i);
        pos.y=start_y + (i * line_height);
        // Draw the line
        DrawTextEx(render->buffer_area.font,line, pos,font_size, 1, render->text_color);
    }
}

void render_status_bar(Renderer *render, Workspace *ws){
    size_t width = render->status_bar.width;
    size_t height = render->status_bar.height;
    size_t pos_x=render->status_bar.pos_x;
    size_t pos_y=render->status_bar.pos_y;
    size_t font_size=render->status_bar.font_size;
    
    DrawRectangle(pos_x, pos_y, width,height, render->status_bar.bg_color);
    size_t text_y=pos_y+(height-font_size)/2;
    Vector2 pos={10,text_y};
    DrawTextEx(render->status_bar.font,ws->working_directory, pos, font_size,1, render->text_color);
}

void render_filepicker(Renderer *render,Workspace *ws){
    size_t pos_x=render->file_picker.pos_x;
    size_t pos_y=render->file_picker.pos_y;
    size_t width=render->file_picker.width;
    size_t height=render->file_picker.height;
    size_t font_size=render->file_picker.font_size;
    
    DrawRectangle(pos_x, pos_y, width, height, render->file_picker.bg_color);
    size_t text_x = render->file_picker.pos_x + render->file_picker.padding_left;
    size_t text_y = render->file_picker.pos_y + render->file_picker.padding_top;
    size_t line_height = render->file_picker.font_size + 5;
    
    Vector2 pos={text_x,text_y};
    
    for(size_t i=0;i<ws->cache_count&&i<20;i++){
        size_t y=text_y+(i*line_height);
        if (i == render->file_picker.selected_index) {
            DrawRectangle(text_x - 5, y - 2, 
                          render->file_picker.width - 10, line_height,
                          render->file_picker.selection_color);
        }
        
        pos.y=y;
        DrawTextEx(render->file_picker.font,ws->cached_paths[i],pos,font_size,1,WHITE);
    }
}

void render_bottom_bar(Renderer *render,Workspace *ws){
    size_t pos_x=render->bottom_bar.pos_x;
    size_t pos_y=render->bottom_bar.pos_y;
    size_t width=render->bottom_bar.width;
    size_t height=render->bottom_bar.height;
    size_t font_size=render->bottom_bar.font_size;
    
    DrawRectangle(pos_x,pos_y,width,height,render->bottom_bar.bg_color);
    
    const char *mode_name=malloc(sizeof(char *));
    strcpy((char *)mode_name,mode_to_text(ws));
    
    size_t text_x=pos_x+5;
    size_t text_y=pos_y+(height-font_size)/2;
    Vector2 pos={text_x,text_y};
    DrawTextEx(render->bottom_bar.font,mode_name,pos,font_size,1,WHITE);
    
    free((char *)mode_name);
}

void render_cursor(Renderer *render,Workspace *ws,Buffer *buffer){
    size_t line_height=render->buffer_area.font_size+4;
    size_t char_width=MeasureTextEx(render->editor_font,"W",render->buffer_area.font_size,1).x;
    size_t cursor_x = render->buffer_area.padding_left + (buffer->cursor.pos_x * char_width);
    size_t cursor_y = render->buffer_area.pos_y + (buffer->cursor.pos_y * line_height);
    if(render->cursor_visible){
        if(ws->current_mode==NORMAL){
            DrawRectangleLines(cursor_x,cursor_y,char_width,line_height,render->cursor_color);
        }else{
            DrawRectangle(cursor_x,cursor_y,char_width,line_height,render->cursor_color);
        }
    }
}

void update_cursor_blink(Renderer *render,double delta_time){
    render->cursor_blink_timer+=delta_time;
    if(render->cursor_blink_timer>=0.5){
        render->cursor_blink_timer=0;
        render->cursor_visible=!render->cursor_visible;
    }
}

void render_switch_focus(Renderer *render,Workspace *ws){
    if (IsKeyPressed(KEY_P)) {
        render->state.display_picker = !render->state.display_picker;
       render->focus=render->state.display_picker? FILEPICKER:BUFFER_AREA; 
       ws->needs_redraw=true;
       return;
    }
}


void process_filepicker_keys(Renderer *render,Workspace *ws){
    if (IsKeyPressed(KEY_UP) && render->file_picker.selected_index > 0)
            render->file_picker.selected_index--;
    if (IsKeyPressed(KEY_DOWN) && render->file_picker.selected_index < ws->cache_count - 1)
            render->file_picker.selected_index++;
        
    if (IsKeyPressed(KEY_ENTER)) {
            const char* path = ws->cached_paths[render->file_picker.selected_index];
            ws_create_buffer(ws, path);

            render->state.display_picker=false;
            render->focus=BUFFER_AREA;
            ws->needs_redraw = true;
    }

    if(IsKeyPressed(KEY_ESCAPE)){
        render->state.display_picker=false;
        render->focus=BUFFER_AREA;
        ws->needs_redraw=true;
    }
}

void process_keys(Renderer *render,Workspace *ws){
    render_switch_focus(render,ws);
    if(render->focus==BUFFER_AREA){
        process_buffer_keys(ws);
    }else if(render->focus==FILEPICKER){
        process_filepicker_keys(render,ws);     
    }
}


