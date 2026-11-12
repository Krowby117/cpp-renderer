//
//  main.h
//  cpp-renderer
//
//  Created by Gavin Dominique on 9/30/26.
//

#pragma once

#include "structs.h"

using namespace std;

// ---- CONSTANTS ---- //
// screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };
constexpr float d{ 500 };

// transformation constants
constexpr int kTransform{ 15 };
constexpr float kScale{ 1.1 };
constexpr float kRotate{ 0.0873 };

// ---- GLOBALS ---- //
SDL_Window* gWindow{ nullptr }; // the window we'll be rendering to
SDL_Renderer* gRenderer{ nullptr }; // the renderer

// active object + list of loaded objects
int active_i = 0;
std::vector<Object> scene;

// ---- MAIN FUNCTIONS ---- //
bool init(); // Starts up SDL and creates the window
void imgui_init(); // Initializes ImGUI
void render(); // Renders the scene
void draw_ui(); // Draws the ImGui ui
void close(); // Frees media and shuts down SDL
void process_input(SDL_Keycode key); // process key press events

// takes in two 2D points and draws a line between them
void render_line(Vec2 s, Vec2 e);
void draw_line_h(Vec2 s, Vec2 e);
void draw_line_v(Vec2 s, Vec2 e);


// ---- HELPER FUNCTIONS ---- //
// takes in a 3D point and converts it into a display coordinate
vector<Vec2> display_polygon(vector<Vec3> polygon);

void new_object(ObjType t);
void remove_object();

// uses the backface culling algorithm to check if a polygon is visible
bool is_visible(vector<Vec3>);

float dot(Vec3 P, Vec3 Q);
Vec3 cross(Vec3 P, Vec3 Q);
Vec3 normalize(Vec3 P);
