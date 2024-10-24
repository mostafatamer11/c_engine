#ifndef DRM_OBJ_H
#define DRM_OBJ_H

#include "render.h"

#define MAX_VERTICES 1024
#define MAX_TRIANGLES 1024


void drmLoadOBJ(const char* filePath, Mesh* mesh);
void drmFreeOBJ(Mesh* mesh);

#endif
