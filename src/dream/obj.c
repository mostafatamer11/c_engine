#include "dream.h"



void drmFreeOBJ(Mesh* mesh) {
	glDeleteVertexArrays(1, &mesh->VAO);
	glDeleteBuffers(1, &mesh->VBO);
	glDeleteBuffers(1, &mesh->EBO);
	glDeleteTextures(1, &mesh->tex->TEX);
	if (mesh->verts != NULL) free(mesh->verts);
	if (mesh->tris != NULL) free(mesh->tris);
}


bool startsWith(const char* pre, const char* str) {
	return strncmp(pre, str, strlen(pre)) == 0;
}

void drmLoadOBJ(const char* filePath, Mesh* mesh) {
	FILE *file = fopen(filePath, "r");
	if (!file) {
		printf("Error: Couldn't open file %s\n", filePath);
		exit(1);
	}

	// Temporary arrays for OBJ data
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

	// Allocate memory for Mesh vertices and triangles based on counts
	size_t vertex_array_size = vertex_count * 2;
	mesh->verts = (Vertex*)malloc(sizeof(Vertex) * vertex_array_size);
	mesh->tris = (Triangle*)malloc(sizeof(Triangle) * triangle_count);

	if (!mesh->verts || !mesh->tris) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}

	// Reset file pointer and counts for second pass
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
					int duplicateIndex = findDuplicateVertex(mesh->verts, vert_index, &vert);
					if (duplicateIndex >= 0) {
					// Use existing vertex if found
						mesh->tris[triangle_count].indices[i] = duplicateIndex;
					} else {
						// Add new vertex if not found
						mesh->verts[vert_index] = vert;
						mesh->tris[triangle_count].indices[i] = vert_index;
						vert_index++;

						// Resize if necessary
						if (vert_index >= vertex_array_size) {
							vertex_array_size *= 2;
							mesh->verts = (Vertex*)realloc(mesh->verts, sizeof(Vertex) * vertex_array_size);
							if (!mesh->verts) {
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
	// Set Mesh lengths to actual counts
	mesh->vertsLen = vert_index;
	mesh->trisLen = triangle_count;
	// Resize to actual size
	mesh->verts = (Vertex*)realloc(mesh->verts, sizeof(Vertex) * mesh->vertsLen);
}


