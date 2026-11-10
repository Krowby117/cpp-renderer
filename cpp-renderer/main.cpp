#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

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
    bool is_running{ true };
    
    // event data
    SDL_Event e;
    SDL_zero(e);
    
    // MAIN LOOP //
    while (is_running == true) {
        // event data code
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) // if quit event, end the render loop
                is_running = false;
            else if (e.type == SDL_EVENT_KEY_DOWN) // if key down event, process input
                process_input(e.key.key);
        }
        
        // rendering code
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        
        draw_ui();
        
        ImGui::Render();
        
        render();
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
    
    // initialize imgui
    imgui_init();
    
    // initialize scene object list
    scene.push_back(Object(CUBE));
    
    return success;
}

void imgui_init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    
    ImGui_ImplSDL3_InitForSDLRenderer(gWindow, gRenderer);
    ImGui_ImplSDLRenderer3_Init(gRenderer);
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
    // clean the screen
    SDL_SetRenderDrawColor(gRenderer, 75, 75, 75, 255);
    SDL_RenderClear(gRenderer);
    
    // iterate through each polygon in the object and grab its points
    int cur_i = 0;
    for (Object obj : scene) {
        for (vector<float> p : obj.polygons) {
            vector<Vec3> polygon;
            for (int i : p) {
                polygon.push_back(obj.vertices[i]);
            }
            
            // if the polygon is not visible, skip it
            if (!is_visible(polygon))
                continue;
            
            // iterate through the points in pairs to draw lines
            for (int i = 0; i < polygon.size(); i++) {
                // grab the start and end point of the line
                Vec3 start = polygon[i];
                Vec3 end = polygon[(i+1) % polygon.size()];
                
                // convert to display coords
                Vec2 d_start = convert_to_display(start);
                Vec2 d_end = convert_to_display(end);
                
                // if active object, use special draw color
                if (cur_i == active_i)
                    SDL_SetRenderDrawColor(gRenderer, 75, 180, 75, 255);
                else
                    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                
                // draw the line
                render_line(d_start, d_end);
            }
        }
        cur_i++;
    }
    
    // update the renderer
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), gRenderer);
    SDL_RenderPresent(gRenderer);
}

void draw_ui() {
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::BeginTable("Object Controls", 4);
    
    // - XYZ Translation Buttons - //
    ImGui::TableNextColumn();
    ImGui::Text("Translations");
    if (ImGui::Button("-X##t"))    scene[active_i].translate({-kTransform,0,0}); ImGui::SameLine();
    if (ImGui::Button("+X##t"))    scene[active_i].translate({kTransform,0,0});
    if (ImGui::Button("-Y##t"))    scene[active_i].translate({0,kTransform,0}); ImGui::SameLine();
    if (ImGui::Button("+Y##t"))    scene[active_i].translate({0,-kTransform,0});
    if (ImGui::Button("-Z##t"))    scene[active_i].translate({0,0,-kTransform}); ImGui::SameLine();
    if (ImGui::Button("+Z##t"))    scene[active_i].translate({0,0,kTransform});

    // - Scaling Buttons - //
    ImGui::TableNextColumn();
    ImGui::Text("Scaling");
    if (ImGui::Button("+##s"))    scene[active_i].scale(kScale); ImGui::SameLine();
    if (ImGui::Button("-##s"))    scene[active_i].scale(1/kScale);

    // - XYZ Rotation Buttons - //
    ImGui::TableNextColumn();
    ImGui::Text("Rotations");
    if (ImGui::Button("-X##r"))    scene[active_i].rotate_x(kRotate); ImGui::SameLine();
    if (ImGui::Button("+X##r"))    scene[active_i].rotate_x(-kRotate);
    if (ImGui::Button("-Y##r"))    scene[active_i].rotate_y(kRotate); ImGui::SameLine();
    if (ImGui::Button("+Y##r"))    scene[active_i].rotate_y(-kRotate);
    if (ImGui::Button("-Z##r"))    scene[active_i].rotate_z(kRotate); ImGui::SameLine();
    if (ImGui::Button("+Z##r"))    scene[active_i].rotate_z(-kRotate);
    
    // - Misc Buttons - //
    ImGui::TableNextColumn();
    ImGui::Text("Miscellaneous");
    if (ImGui::Button("Reset##m"))    scene[active_i].reset();
    if (ImGui::Button("Swith##m"))    active_i = (active_i + 1) % scene.size(); ImGui::SameLine();

    ImGui::EndTable();
    ImGui::End();
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

bool is_visible(vector<Vec3> polygon) {
    // Step 1 : Compute the normal vector of the polygon, then normalize it
    Vec3 p0 = polygon[0];
    Vec3 p1 = polygon[1];
    Vec3 p2 = polygon[2];
    
    // compute P and Q
    Vec3 P = p1 - p0;
    Vec3 Q = p2 - p0;
    
    // cross P and Q to get the surface normal and then normalize it
    Vec3 N = cross(P, Q);
    
    float den = sqrt((N.x * N.x) + (N.y * N.y) + (N.z * N.z));
    if (den == 0)
        return false;
        
    Vec3 norm = {
        N.x / den,
        N.y / den,
        N.z / den
    };
    
    // Step 2: Compute the Plane Offset
    float D = dot(norm, p0);
    
    
    // Determine the visibility of the plane from the viewpoint
    float test = dot(norm, {0,0,-d}) - D;
    if (test > 0)
        return true;
    else
        return false;
}

float dot(Vec3 P, Vec3 Q) {
    return (P.x * Q.x) + (P.y * Q.y) + (P.z * Q.z);
}

Vec3 cross(Vec3 P, Vec3 Q) {
    return {
        (P.y * Q.z) - (P.z * Q.y),
        (P.z * Q.x) - (P.x * Q.z),
        (P.x * Q.y) - (P.y * Q.x)
    };
}

Vec3 normalize(Vec3 P) {
    float x = P.x * P.x;
    float y = P.y * P.y;
    float z = P.z * P.z;
    
    float den = sqrt(x + y + z);
    
    return {
        P.x / den,
        P.y / den,
        P.z / den
    };
}

void process_input(SDL_Keycode key) {
    Object* obj = &scene[active_i];
    
    // change active object
    if (key == SDLK_TAB)    active_i = (active_i + 1) % scene.size();
    
    // reset object
    if (key == SDLK_R)      obj->reset();
    
    // left and right translations
    if (key == SDLK_LEFT)   obj->translate({-kTransform,0,0});
    if (key == SDLK_RIGHT)  obj->translate({kTransform,0,0});
    if (key == SDLK_UP)     obj->translate({0,kTransform,0});
    if (key == SDLK_DOWN)   obj->translate({0,-kTransform,0});
    
    // larger and smaller scaling
    if (key == SDLK_Q)      obj->scale(kScale);
    if (key == SDLK_E)      obj->scale(1/kScale);
    
    // x rotation
    if (key == SDLK_1)      obj->rotate_x(kRotate);
    if (key == SDLK_2)      obj->rotate_x(-kRotate);
    
    // y rotation
    if (key == SDLK_3)      obj->rotate_y(kRotate);
    if (key == SDLK_4)      obj->rotate_y(-kRotate);
    
    // z rotation
    if (key == SDLK_5)      obj->rotate_z(kRotate);
    if (key == SDLK_6)      obj->rotate_z(-kRotate);
}
