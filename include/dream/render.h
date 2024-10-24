#ifndef DRM_RENDER_H
#define DRM_RENDER_H
#include "dream.h" 

void drmInitOBJ(Mesh* cube);
void drmRenderOBJ(unsigned int *shaderProgram, Mesh* mesh, Camera* camera);

#endif
