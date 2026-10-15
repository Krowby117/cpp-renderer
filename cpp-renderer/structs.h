//
//  structs.h
//  cpp-renderer
//
//  Created by Gavin Dominique on 10/15/26.
//

#pragma once

// -- Vector Structs -- //
struct Vec2 {
    float x, y;
    
    Vec2(float x, float y);
};

struct Vec3 {
    float x, y, z;
    
    Vec3(float x, float y, float z);
};

struct Vec4 {
    float x, y, z, w;
    
    Vec4(float x, float y, float z, float w);
    Vec4(float x, float y, float z);
};

// -- Object Struct -- //
enum ObjType {
    CUBE,
    PYRAMID
};
struct Object {
    // collection of object's verts and polygons
    std::vector<Vec3> vertices;
    std::vector<std::vector<float>> polygons;

    // copy of objects vertices without transformations
    std::vector<Vec3> orig_vertices;
    
    Object(ObjType type);
    
    // helper functions
    std::vector<float> get_reference_point();
    void reset();
    
    // translation
    void translate(std::vector<float> t);
    void perm_translate(std::vector<float> t);
    
    // scaling
    void scale(float s);
    
    // rotations
    void rotate_x(float a);
    void rotate_y(float a);
    void rotate_z(float a);
};

