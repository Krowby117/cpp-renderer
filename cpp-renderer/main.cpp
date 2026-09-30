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
    
    s1.translate({-125,0,0});
    s2.translate({125,0,0});
    
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
    for (std::vector<float> p : s2.polygons) {
        std::vector<Vec3> polygon;
        for (int i : p) {
            polygon.push_back(s2.points[i]);
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
            draw_line(d_start, d_end);
        }
    }
    
    // iterate through each polygon in the object and grab its points
    for (std::vector<float> p : s1.polygons) {
        std::vector<Vec3> polygon;
        for (int i : p) {
            polygon.push_back(s1.points[i]);
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
            if (SDL_RenderLine(gRenderer, d_start.x, d_start.y, d_end.x, d_end.y)== false) {SDL_Log("did not draw line: %s\n", SDL_GetError());}
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
void draw_line(Vec2 s, Vec2 e) {
    int dx = e.x - s.x;
    int dy = e.y - s.y;
    int D = 2*dy - dx;
    int y = s.y;
    
    for (int x = s.x; x <= e.x; x++) {
        SDL_RenderPoint(gRenderer, x, y);
        if (D > 0) {
            y = y + 1;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2*dy;
        }
    }
}

void process_input(SDL_Keycode key) {
    // reset object
    if (key == SDLK_R) { s1.reset(); }
    
    // left and right translations
    if (key == SDLK_LEFT) { s1.translate({-kTransform,0,0}); }
    if (key == SDLK_RIGHT) { s1.translate({kTransform,0,0}); }
    if (key == SDLK_UP) { s1.translate({0,kTransform,0}); }
    if (key == SDLK_DOWN) { s1.translate({0,-kTransform,0}); }
    
    // larger and smaller scaling
    if (key == SDLK_Q) { s1.scale(kScale); }
    if (key == SDLK_E) { s1.scale(1/kScale); }
    
    // x rotation
    if (key == SDLK_1) { s1.rotate_x(5);}
    if (key == SDLK_2) { s1.rotate_x(-5);}
    
    // y rotation
    if (key == SDLK_3) { s1.rotate_y(5); s2.rotate_y(5);}
    if (key == SDLK_4) { s1.rotate_y(-5); s2.rotate_y(-5);}
    
    // z rotation
    if (key == SDLK_5) { s1.rotate_z(5);}
    if (key == SDLK_6) { s1.rotate_z(-5);}
}
