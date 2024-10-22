#ifndef GEO_H
#define GEO_H

#include <engine.h>

typedef enum {
    VERTEX_PLAIN,
    VERTEX_NORMAL_TEXCOORD
} VertexType;

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


void initVert(Vertex* vert, VertexType t, float* pos, float* col, float* normal, float* texcoord);
void printVert(Vertex* vert);


#endif
