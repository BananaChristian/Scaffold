#include "raylib.h"
#include "renderer.h"
#include "workspace.h"

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1200,600,"Scaff");
    SetTargetFPS(60);
    
    Workspace workspace;
    Renderer render;

    ws_init(&workspace);
    render_init(&render);
    
    while(!WindowShouldClose()){
        if(IsWindowResized()){
            render_calculate_layout(&render);  // Recalculate all components
             workspace.needs_redraw = true;
        }
        render_draw(&render,&workspace);
    }
    
    CloseWindow();
    ws_destroy(&workspace);
    return 0;
}
