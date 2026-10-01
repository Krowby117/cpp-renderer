#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <vector>
#include <iostream>

#include "main.h"

//////////////////// MAIN ////////////////////
int main(int argc, char* args[])
{
    // initialize
    if (init() == false) {
        SDL_Log("Unable to initialize program.\n");
        return 1;
    }
    
    // quit program signal
    bool quit{ false };
    
    // event data
    SDL_Event e;
    SDL_zero(e);
    
    // MAIN LOOP //
    while (quit == false) {
        // get event data
        while (SDL_PollEvent(&e) == true ) {
            if (e.type == SDL_EVENT_QUIT) // if the event is a quit type, end the main loop
                quit = true;
            else if (e.type == SDL_EVENT_KEY_DOWN) // if key down event, process input
                process_input(e.key.key);
        }
        
        // clean the screen
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set the color used for drawing operations.
        SDL_RenderClear(gRenderer); // Clear the current rendering target with the drawing color.
        
        // render object(s)
        render();
        
        // update the renderer
        SDL_RenderPresent(gRenderer);
    }
    
    // clean up and end the program
    close();
    return 0;
}
//////////////////////////////////////////////

//* Function Implemetations *//
bool init()
{
    // initialization flag
    bool success { true };
    
    // initiale SDL
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        success = false;
    } else {
        // create the window and renderer
        if (SDL_CreateWindowAndRenderer("SDL Renderer", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer) == false)
        {
            SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
            success = false;
        }
    }
    
    return success;
}

void close()
{
    // destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;
    
    // quit SDL subsystems
    SDL_Quit();
}

void render()
{
    // iterate through each polygon in the object and grab its points
    for (std::vector<float> p : obj.polygons) {
        std::vector<Vec3> polygon;
        for (int i : p) {
            polygon.push_back(obj.points[i]);
        }
        
        // iterate through the points in pairs to draw lines
        for (int i = 0; i < polygon.size(); i++) {
            // grab the start and end point of the line
            Vec3 start = polygon[i];
            Vec3 end = polygon[(i+1) % polygon.size()];
            
            // convert to display coords
            Vec2 d_start = convert_to_display(start);
            Vec2 d_end = convert_to_display(end);
            
            // draw the line
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
            SDL_SetRenderColorScale(gRenderer, 1.0);
            render_line(d_start, d_end);
        }
    }
}

Vec2 convert_to_display(Vec3 p) {
    // project the 3D point onto the XY plane
    float den = d + p.z;
    Vec3 proj = {
        d * (p.x/ den),
        d * (p.y/ den),
        d * (p.z / den)
    };
    
    // convert to display coordinates
    Vec2 disp = {
        (kScreenWidth / 2) + proj.x,
        (kScreenHeight / 2) - proj.y
    };
    
    // return the display coords
    return disp;
}

// implements Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
// https://www.youtube.com/watch?v=CceepU1vIKo
void render_line(Vec2 s, Vec2 e) {
    if (abs(e.x-s.x) > abs(e.y-s.y)) {
        draw_line_h(s, e);
    } else {
        draw_line_v(s, e);
    }
}
void draw_line_h(Vec2 s, Vec2 e) {
    // if end point is before the start point, swap them
    if (s.x > e.x) {
        Vec2 temp = {s.x, s.y};
        s = {e.x, e.y};
        e = {temp.x, temp.y};
    }
    
    int dx = e.x - s.x;
    int dy = e.y - s.y;
    
    int dir = (dy < 0) ? -1 : 1;
    dy *= dir;
    
    if (dx != 0) {
        int y = s.y;
        int p = 2*dy - dx;
        
        for (int i = 0; i <= dx; i++) {
            SDL_RenderPoint(gRenderer, s.x + i, y);
            if (p >= 0) {
                y += dir;
                p -= 2*dx;
            }
            
            p += 2*dy;
        }
    }
}
void draw_line_v(Vec2 s, Vec2 e) {
    // if end point is before the start point, swap them
    if (s.y > e.y) {
        Vec2 temp = {s.x, s.y};
        s = {e.x, e.y};
        e = {temp.x, temp.y};
    }
    
    int dx = e.x - s.x;
    int dy = e.y - s.y;
    
    int dir = (dx < 0) ? -1 : 1;
    dx *= dir;
    
    if (dy != 0) {
        int x = s.x;
        int p = 2*dx - dy;
        
        for (int i = 0; i <= dy; i++) {
            SDL_RenderPoint(gRenderer, x, s.y + i);
            if (p >= 0) {
                x += dir;
                p -= 2*dy;
            }
            
            p += 2*dx;
        }
    }
}

void process_input(SDL_Keycode key) {
    // reset object
    if (key == SDLK_R) { obj.reset(); }
    
    // left and right translations
    if (key == SDLK_LEFT) { obj.translate({-kTransform,0,0}); }
    if (key == SDLK_RIGHT) { obj.translate({kTransform,0,0}); }
    if (key == SDLK_UP) { obj.translate({0,kTransform,0}); }
    if (key == SDLK_DOWN) { obj.translate({0,-kTransform,0}); }
    
    // larger and smaller scaling
    if (key == SDLK_Q) { obj.scale(kScale); }
    if (key == SDLK_E) { obj.scale(1/kScale); }
    
    // x rotation
    if (key == SDLK_1) { obj.rotate_x(5);}
    if (key == SDLK_2) { obj.rotate_x(-5);}
    
    // y rotation
    if (key == SDLK_3) { obj.rotate_y(5);}
    if (key == SDLK_4) { obj.rotate_y(-5);}
    
    // z rotation
    if (key == SDLK_5) { obj.rotate_z(5);}
    if (key == SDLK_6) { obj.rotate_z(-5);}
}
