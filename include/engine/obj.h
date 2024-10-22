#ifndef OBJ_H
#define OBJ_H

#include "engine.h"

#define MAX_VERTICES 1024
#define MAX_TRIANGLES 1024


void loadOBJ(const char* filePath, Vertex** verts, int* vertLen, Triangle** tris, int* triLen);
void freeOBJ(Vertex* verts, Triangle* tris);

#endif
