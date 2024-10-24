#ifndef DRM_GEO_H
#define DRM_GEO_H

#include "dream.h"

typedef enum {
    VERTEX_PLAIN,
    VERTEX_NORMAL_TEXCOORD
} VertexType;


typedef struct {
	int width, height, colNum;
	unsigned char* bytes;
	unsigned int TEX;
} Texture;

typedef struct {
	float pos[3];
	union {
		struct {
			float col[3];
		};
		struct {
			float normal[3];
			float texcoord[2];
		};
	};
	VertexType type;
} Vertex;


typedef struct {
	unsigned int indices[3];
} Triangle;

typedef struct {
	size_t vertsLen;
	size_t trisLen;
	Vertex* verts;
	Triangle* tris;
	unsigned int VBO, VAO, EBO;
	Texture* tex;
} Mesh;


void drmInitVert(Vertex* vert, VertexType t, float* pos, float* col, float* normal, float* texcoord);
void drmIrintVert(Vertex* vert);

#endif
