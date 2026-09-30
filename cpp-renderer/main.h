//
//  main.h
//  cpp-renderer
//
//  Created by Gavin Dominique on 9/30/26.
//

#ifndef main_h
#define main_h

#include "structs.cpp"

// screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };
constexpr int d{ 500 };

// transformation constants
constexpr int kTransform{ 15 };
constexpr float kScale{ 1.1 };
constexpr int kRotate{ 30 };


bool init(); // Starts up SDL and creates the window
void render(); // Render objects onto the screen
void close(); // Frees media and shuts down SDL
void process_input(SDL_Keycode key); // process key press events

// takes in two 2D points and draws a line between them
void draw_line(Vec2 s, Vec2 e);

// takes in a 3D point and converts it into a display coordinate (projects onto XY then coverts to display)
Vec2 convert_to_display(Vec3 p);

SDL_Window* gWindow{ nullptr }; // the window we'll be rendering to
SDL_Renderer* gRenderer{ nullptr }; // the renderer

// active object
Object s1;
Object s2;

#endif /* main_h */
