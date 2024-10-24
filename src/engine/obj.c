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
    size_t vertex_array_size = vertex_count * 2;
    Vertex* vertexList = (Vertex*)malloc(sizeof(Vertex) * vertex_array_size);
    *tris = (Triangle*)malloc(sizeof(Triangle) * triangle_count);

    if (!vertexList || !(*tris)) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    // Reset file pointer to the beginning and reset counts
    rewind(file);
    vertex_count = texcoord_count = normal_count = 0;
    int vert_index = 0;
    triangle_count = 0;

    // Helper function to check for duplicate vertices
    int findDuplicateVertex(Vertex* vertexList, int vert_index, Vertex* vert) {
        for (int i = 0; i < vert_index; i++) {
            if (memcmp(&vertexList[i], vert, sizeof(Vertex)) == 0) {
                return i;
            }
        }
        return -1;
    }

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
                    Vertex vert;
                    vert.pos[0] = temp_vertices[v_idx[i] - 1][0];
                    vert.pos[1] = temp_vertices[v_idx[i] - 1][1];
                    vert.pos[2] = temp_vertices[v_idx[i] - 1][2];
                    if (vt_idx[i] > 0) {
                        vert.texcoord[0] = temp_texcoords[vt_idx[i] - 1][0];
                        vert.texcoord[1] = temp_texcoords[vt_idx[i] - 1][1];
                    } else {
                        vert.texcoord[0] = vert.texcoord[1] = 0.0f;
                    }
                    if (vn_idx[i] > 0) {
                        vert.normal[0] = temp_normals[vn_idx[i] - 1][0];
                        vert.normal[1] = temp_normals[vn_idx[i] - 1][1];
                        vert.normal[2] = temp_normals[vn_idx[i] - 1][2];
                    }
                    vert.type = VERTEX_NORMAL_TEXCOORD;

                    // Check if this vertex already exists
                    int duplicateIndex = findDuplicateVertex(vertexList, vert_index, &vert);
                    if (duplicateIndex >= 0) {
                        // Use existing vertex if found
                        (*tris)[triangle_count].indices[i] = duplicateIndex;
                    } else {
                        // Add new vertex if not found
                        vertexList[vert_index] = vert;
                        (*tris)[triangle_count].indices[i] = vert_index;
                        vert_index++;

                        // Resize if necessary
                        if (vert_index >= vertex_array_size) {
                            vertex_array_size *= 2;
                            vertexList = (Vertex*)realloc(vertexList, sizeof(Vertex) * vertex_array_size);
                            if (!vertexList) {
                                printf("Error: Memory allocation failed\n");
                                exit(1);
                            }
                        }
                    }
                }
                triangle_count++;
            }
        }
    }

    fclose(file);

    *vertLen = vert_index;
    *verts = (Vertex*)realloc(vertexList, sizeof(Vertex) * vert_index); // Resize to actual size
    *triLen = triangle_count;
}

