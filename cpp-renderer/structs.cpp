//
//  structs.cpp
//  cpp-renderer
//
//  Created by Gavin Dominique on 9/30/26.
//

#include <vector>
#include "structs.h"

// -- Vector Structs -- //
Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
    
Vec4::Vec4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
    
Vec4::Vec4(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1;
}

// -- PYRAMID VERTICES AND POLYGONS --//
std::vector<Vec3> pyramid_verts = {
    {0, 50, 100},
    {50, -50, 50},
    {50, -50, 150},
    {-50, -50, 150},
    {-50, -50, 50}
};
std::vector<std::vector<float>> pyramid_polygons = {
    {0, 1, 4},          // frontpoly
    {0, 2, 1},          // rightpoly
    {0, 3, 2},          // backpoly
    {0, 4, 3},          // leftpoly
    {1, 2, 3, 4}        // bottompoly
};

// -- CUBE VERTICES AND POLYGONS --//
std::vector<Vec3> cube_verts = {
    {50, 50, 50},
    {50, 50, 150},
    {-50, 50, 150},
    {-50, 50, 50},
    {50, -50, 50},
    {50, -50, 150},
    {-50, -50, 150},
    {-50, -50, 50},
};
std::vector<std::vector<float>> cube_polygons = {
    {4, 0, 3, 7},          // frontpoly
    {0, 1, 5, 4},          // rightpoly
    {5, 1, 2, 6},          // backpoly
    {3, 2, 6, 7},          // leftpoly
    {0, 1, 2, 3},          // toppoly
    {4, 5, 6, 7}           // bottompoly
};

// -- Object Struct -- //
Object::Object(ObjType type) {
    switch (type) {
        case CUBE:
            vertices = orig_vertices = cube_verts;
            polygons = cube_polygons;
            break;
        case PYRAMID:
            vertices = orig_vertices = pyramid_verts;
            polygons = pyramid_polygons;
            break;
    }
}
    
std::vector<float> Object::get_reference_point() {
    std::vector<float> min = {10000000,10000000,10000000};
    std::vector<float> max = {-10000000, -10000000, -10000000};
    
    // get the min and max values
    for (Vec3 p : vertices) {
        // compare and grab the min value
        min[0] = (p.x < min[0]) ? p.x : min[0];
        min[1] = (p.y < min[1]) ? p.y : min[1];
        min[2] = (p.z < min[2]) ? p.z : min[2];
        
        // compare and grab the max value
        max[0] = (p.x > max[0]) ? p.x : max[0];
        max[1] = (p.y > max[1]) ? p.y : max[1];
        max[2] = (p.z > max[2]) ? p.z : max[2];
    }
    
    // calculate and return the reference value
    return {
        (min[0] + max[0]) / 2,
        (min[1] + max[1]) / 2,
        (min[2] + max[2]) / 2
    };
}
    
void Object::reset() {
    vertices = orig_vertices;
}
    
void Object::translate(std::vector<float> t) {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].x += t[0];
        vertices[i].y += t[1];
        vertices[i].z += t[2];
    }
}

void Object::perm_translate(std::vector<float> t) {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].x += t[0];
        vertices[i].y += t[1];
        vertices[i].z += t[2];
        
        orig_vertices[i].x += t[0];
        orig_vertices[i].y += t[1];
        orig_vertices[i].z += t[2];
    }
}

void Object::scale(float s) {
    // translate to origin
    std::vector<float> ref = get_reference_point();
    translate({-ref[0], -ref[1], -ref[2]});
    
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].x *= s;
        vertices[i].y *= s;
        vertices[i].z *= s;
    }
    
    // translate back to original location
    translate(ref);
}

void Object::rotate_x(float a) {
    // translate to origin
    std::vector<float> ref = get_reference_point();
    translate({-ref[0], -ref[1], -ref[2]});
    
    // cos sin values
    float c = cos(a);
    float s = sin(a);
    
    for (int i = 0; i < vertices.size(); i++) {
        // grab copies of original points
        float y = vertices[i].y;
        float z = vertices[i].z;
        
        vertices[i].y = (y * c) - (z * s);
        vertices[i].z = (y * s) + (z * c);
    }
    
    // translate back to original location
    translate(ref);
}

void Object::rotate_y(float a) {
    // translate to origin
    std::vector<float> ref = get_reference_point();
    translate({-ref[0], -ref[1], -ref[2]});
    
    // cos sin values
    float c = cos(a);
    float s = sin(a);
    
    for (int i = 0; i < vertices.size(); i++) {
        // grab copies of original points
        float x = vertices[i].x;
        float z = vertices[i].z;
        
        vertices[i].x = (x * c) + (z * s);
        vertices[i].z = -(x * s) + (z * c);
    }
    
    // translate back to original location
    translate(ref);
}

void Object::rotate_z(float a) {
    // translate to origin
    std::vector<float> ref = get_reference_point();
    translate({-ref[0], -ref[1], -ref[2]});
    
    // cos sin values
    float c = cos(a);
    float s = sin(a);
    
    for (int i = 0; i < vertices.size(); i++) {
        // grab copies of original points
        float x = vertices[i].x;
        float y = vertices[i].y;
        
        vertices[i].x = (x * c) - (y * s);
        vertices[i].y = (x * s) + (y * c);
    }
    
    // translate back to original location
    translate(ref);
}
