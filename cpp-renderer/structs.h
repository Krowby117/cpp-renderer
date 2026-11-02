//
//  structs.h
//  cpp-renderer
//
//  Created by Gavin Dominique on 10/15/26.
//

#pragma once

using namespace std;

// -- Vector Structs -- //
struct Vec2 {
    float x, y;
    
    Vec2();
    Vec2(float x, float y);
};

struct Vec3 {
    float x, y, z;
    
    Vec3();
    Vec3(float x, float y, float z);
    
    Vec3 operator-(const Vec3 &n)
    {
        return {
            this->x - n.x,
            this->y - n.y,
            this->z - n.z
        };
    }
};

struct Vec4 {
    float x, y, z, w;
    
    Vec4();
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
    vector<Vec3> vertices;
    vector<vector<float>> polygons;

    // copy of objects vertices without transformations
    std::vector<Vec3> orig_vertices;
    
    Object(ObjType type);
    
    // helper functions
    vector<float> get_reference_point();
    void reset();
    
    // translation
    void translate(vector<float> t);
    void perm_translate(vector<float> t);
    
    // scaling
    void scale(float s);
    
    // rotations
    void rotate_x(float a);
    void rotate_y(float a);
    void rotate_z(float a);
};

