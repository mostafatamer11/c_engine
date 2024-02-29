#ifndef DRAW_H
#define DRAW_H

#include <glad/glad.h>
#include <math.h>


typedef struct {
    float x, y, z;
    float r, g, b;
    float nx, ny, nz; // Normal components
} Vertex;

typedef struct {
    Vertex verts[3];
} Triangle;

void calculateNormals(Vertex* v1, Vertex* v2, Vertex* v3);
void draw_simple_triangle(Triangle* triangle);
void createCubeMesh(Vertex cubeVertices[8], Triangle cubeTriangles[12]);
void draw_mesh(Triangle tris[], int size);

#endif