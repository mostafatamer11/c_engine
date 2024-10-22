#include "engine.h"



void freeOBJ(Vertex* verts, Triangle* tris) {
    if (verts != NULL) {
        free(verts);
    }
    if (tris != NULL) {
        free(tris);
    }
}


bool startsWith(const char* pre, const char* str) {
	return strncmp(pre, str, strlen(pre)) == 0;
}


void loadOBJ(const char* filePath, Vertex** verts, int* vertLen, Triangle** tris, int* triLen) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        printf("Error: Couldn't open file %s\n", filePath);
        exit(1);
    }

    // Temporary buffers for vertices, texture coords, and normals
    float temp_vertices[MAX_VERTICES][3];
    float temp_texcoords[MAX_VERTICES][2];
    float temp_normals[MAX_VERTICES][3];

    int vertex_count = 0, texcoord_count = 0, normal_count = 0;
    int triangle_count = 0;

    char line[128];

    // First pass to count vertices and faces
    while (fgets(line, sizeof(line), file)) {
        if (startsWith("v ", line)) {
            vertex_count++;
        } else if (startsWith("vt", line)) {
            texcoord_count++;
        } else if (startsWith("vn", line)) {
            normal_count++;
        } else if (startsWith("f ", line)) {
            triangle_count++;
        }
    }

    // Allocate memory based on counts
    *verts = (Vertex*)malloc(sizeof(Vertex) * vertex_count);
    *tris = (Triangle*)malloc(sizeof(Triangle) * triangle_count);
    if (!(*verts) || !(*tris)) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    // Reset file pointer to the beginning and reset counts
    rewind(file);
    vertex_count = texcoord_count = normal_count = 0;
    triangle_count = 0;

    // Second pass to populate vertex and triangle data
    while (fgets(line, sizeof(line), file)) {
        if (startsWith("v ", line)) {
            sscanf(line, "v %f %f %f", &temp_vertices[vertex_count][0], &temp_vertices[vertex_count][1], &temp_vertices[vertex_count][2]);
            vertex_count++;
        } else if (startsWith("vt", line)) {
            sscanf(line, "vt %f %f", &temp_texcoords[texcoord_count][0], &temp_texcoords[texcoord_count][1]);
            texcoord_count++;
        } else if (startsWith("vn", line)) {
            sscanf(line, "vn %f %f %f", &temp_normals[normal_count][0], &temp_normals[normal_count][1], &temp_normals[normal_count][2]);
            normal_count++;
        } else if (startsWith("f ", line)) {
            int v_idx[3], vt_idx[3], vn_idx[3];
            int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                                 &v_idx[0], &vt_idx[0], &vn_idx[0],
                                 &v_idx[1], &vt_idx[1], &vn_idx[1],
                                 &v_idx[2], &vt_idx[2], &vn_idx[2]);

            if (matches == 9) {
                for (int i = 0; i < 3; i++) {
                    Vertex vertex;
                    vertex.pos[0] = temp_vertices[v_idx[i] - 1][0];
                    vertex.pos[1] = temp_vertices[v_idx[i] - 1][1];
                    vertex.pos[2] = temp_vertices[v_idx[i] - 1][2];

                    if (vt_idx[i] > 0 && texcoord_count > 0) {
                        vertex.texcoord[0] = temp_texcoords[vt_idx[i] - 1][0];
                        vertex.texcoord[1] = temp_texcoords[vt_idx[i] - 1][1];
                    } else {
                        vertex.texcoord[0] = vertex.texcoord[1] = 0.0f;  // Default value
                    }

                    if (vn_idx[i] > 0 && normal_count > 0) {
                        vertex.normal[0] = temp_normals[vn_idx[i] - 1][0];
                        vertex.normal[1] = temp_normals[vn_idx[i] - 1][1];
                        vertex.normal[2] = temp_normals[vn_idx[i] - 1][2];
                        vertex.type = VERTEX_NORMAL_TEXCOORD;
                    } else {
                        vertex.type = VERTEX_PLAIN;
                    }

                    (*verts)[v_idx[i] - 1] = vertex;  // Assign vertex
                }
            }

            Triangle triangle = {
                .indices = { v_idx[0] - 1, v_idx[1] - 1, v_idx[2] - 1 }
            };
            (*tris)[triangle_count++] = triangle;  // Add triangle
        }
    }

    fclose(file);

    // Set the counts after processing
    *vertLen = vertex_count;
    *triLen = triangle_count;
}

