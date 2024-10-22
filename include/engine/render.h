#ifndef RENDER_H
#define RENDER_H
#include "engine.h" 

void initOBJ(Vertex *vertices, Triangle *indices, size_t vertexCount, size_t indexCount, unsigned int *VBO, unsigned int *EBO, unsigned int *VAO);
void renderOBJ(unsigned int *shaderProgram, unsigned int *VAO, Camera* camera, unsigned long tris_count);

#endif
