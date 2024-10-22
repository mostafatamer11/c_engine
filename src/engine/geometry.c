#include "engine.h"
#include <engine/geometry.h>


void initVert(Vertex* vert, VertexType t, float* pos, float* col, float* normal, float* texcoord) {
    vert->type = t;

    // Initialize the position (if provided)
    if (pos != NULL) {
        vert->pos[0] = pos[0];
        vert->pos[1] = pos[1];
        vert->pos[2] = pos[2];
    } else {
        vert->pos[0] = vert->pos[1] = vert->pos[2] = 0.0f;
    }

    // Handle based on vertex type
    if (t == VERTEX_PLAIN && col != NULL) {
        vert->col[0] = col[0];
        vert->col[1] = col[1];
        vert->col[2] = col[2];
    } else if (t == VERTEX_NORMAL_TEXCOORD && normal != NULL && texcoord != NULL) {
        vert->normal[0] = normal[0];
        vert->normal[1] = normal[1];
        vert->normal[2] = normal[2];

        vert->texcoord[0] = texcoord[0];
        vert->texcoord[1] = texcoord[1];
    }
}


void printVert(Vertex* vert) {
	printf("Position: (%f, %f, %f)\n", vert->pos[0], vert->pos[1], vert->pos[2]);
	printf("Color: (%f, %f, %f)\n", vert->col[0], vert->col[1], vert->col[2]);
	printf("Normal: (%f, %f, %f)\n", vert->normal[0], vert->normal[1], vert->normal[2]);
	printf("Texcoord: (%f, %f)\n", vert->texcoord[0], vert->texcoord[1]);
}

