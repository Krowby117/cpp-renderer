//
//  structs.cpp
//  cpp-renderer
//
//  Created by Gavin Dominique on 9/30/26.
//

#include <vector>

//* Vector Structs *//
struct Vec2 {
    float x, y;
    
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

struct Vec3 {
    float x, y, z;
    
    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

//* Object Struct *//
struct Object {
    // Definition of the underlying points
    std::vector<Vec3> points = {
        {75, 75, -75},
        {75, 75, 75},
        {-75, 75, 75},
        {-75, 75, -75},
        {75, -75, -75},
        {75, -75, 75},
        {-75, -75, 75},
        {-75, -75, -75},
    };

    // Definition of the five polygon faces
    std::vector<std::vector<float>> polygons = {
        {4, 0, 3, 7},          // frontpoly
        {0, 1, 5, 4},          // rightpoly
        {5, 1, 2, 6},          // backpoly
        {3, 2, 6, 7},          // leftpoly
        {0, 1, 2, 3},          // toppoly
        {4, 5, 6, 7}           // bottompoly
    };
    
    std::vector<float> referencePoint() {
        std::vector<float> min = {10000000,10000000,10000000};
        std::vector<float> max = {-10000000, -10000000, -10000000};
        
        // get the min and max values
        for (Vec3 p : points) {
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
    
    void reset() {
        points = {
            {75, 75, -75},
            {75, 75, 75},
            {-75, 75, 75},
            {-75, 75, -75},
            {75, -75, -75},
            {75, -75, 75},
            {-75, -75, 75},
            {-75, -75, -75},
        };
    }
    
    void translate(std::vector<float> t) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x += t[0];
            points[i].y += t[1];
            points[i].z += t[2];
        }
    }
    
    void scale(float s) {
        std::vector<float> ref = referencePoint();
        
        for (int i = 0; i < points.size(); i++) {
            // translate to origin
            translate({-ref[0], -ref[1], -ref[2]});
            
            // scale
            points[i].x *= s;
            points[i].y *= s;
            points[i].z *= s;
            
            // translate back to spot
            translate(ref);
        }
    }
    
    void rotate_x(float a) {
        std::vector<float> ref = referencePoint();
        
        for (int i = 0; i < points.size(); i++) {
            // translate to origin
            translate({-ref[0], -ref[1], -ref[2]});
            
            // cos sin values
            float c = cos(a);
            float s = sin(a);

            // grab copies of original points
            float y = points[i].y;
            float z = points[i].z;
            
            points[i].y = (y * c) - (z * s);
            points[i].z = (y * s) + (z * c);
            
            // translate back to spot
            translate(ref);
        }
    }
    
    void rotate_y(float a) {
        std::vector<float> ref = referencePoint();
        
        for (int i = 0; i < points.size(); i++) {
            // translate to origin
            translate({-ref[0], -ref[1], -ref[2]});
            
            // cos sin values
            float c = cos(a);
            float s = sin(a);

            // grab copies of original points
            float x = points[i].x;
            float z = points[i].z;
            
            points[i].x = (x * c) + (z * s);
            points[i].z = -(x * s) + (z * c);
            
            // translate back to spot
            translate(ref);
        }
    }
    
    void rotate_z(float a) {
        std::vector<float> ref = referencePoint();
        
        for (int i = 0; i < points.size(); i++) {
            // translate to origin
            translate({-ref[0], -ref[1], -ref[2]});
            
            // cos sin values
            float c = cos(a);
            float s = sin(a);

            // grab copies of original points
            float x = points[i].x;
            float y = points[i].y;
            
            points[i].x = (x * c) - (y * s);
            points[i].y = (x * s) + (y * c);
            
            // translate back to spot
            translate(ref);
        }
    }
};
