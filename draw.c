#include "draw.h"

void calculateNormals(Vertex* v1, Vertex* v2, Vertex* v3) {
    // Calculate face normal
    float normalX = (v2->y - v1->y) * (v3->z - v1->z) - (v2->z - v1->z) * (v3->y - v1->y);
    float normalY = (v2->z - v1->z) * (v3->x - v1->x) - (v2->x - v1->x) * (v3->z - v1->z);
    float normalZ = (v2->x - v1->x) * (v3->y - v1->y) - (v2->y - v1->y) * (v3->x - v1->x);

    // Normalize the result
    float length = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
    v1->nx = normalX / length;
    v1->ny = normalY / length;
    v1->nz = normalZ / length;

    v2->nx = v1->nx;
    v2->ny = v1->ny;
    v2->nz = v1->nz;

    v3->nx = v1->nx;
    v3->ny = v1->ny;
    v3->nz = v1->nz;
}

void draw_simple_triangle(Triangle* triangle) {
    GLuint vao, vbo;

    GLfloat vertices[] = {
        triangle->verts[0].x, triangle->verts[0].y, triangle->verts[0].z, triangle->verts[0].r, triangle->verts[0].g, triangle->verts[0].b, triangle->verts[0].nx, triangle->verts[0].ny, triangle->verts[0].nz,
        triangle->verts[1].x, triangle->verts[1].y, triangle->verts[1].z, triangle->verts[1].r, triangle->verts[1].g, triangle->verts[1].b, triangle->verts[1].nx, triangle->verts[1].ny, triangle->verts[1].nz,
        triangle->verts[2].x, triangle->verts[2].y, triangle->verts[2].z, triangle->verts[2].r, triangle->verts[2].g, triangle->verts[2].b, triangle->verts[2].nx, triangle->verts[2].ny, triangle->verts[2].nz
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void createCubeMesh(Vertex cubeVertices[8], Triangle cubeTriangles[12]) {
    // Define vertices of the cube with each edge having a length of 1
    cubeVertices[0] = (Vertex){-0.5,  0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[1] = (Vertex){-0.5, -0.5,  0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[2] = (Vertex){ 0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[3] = (Vertex){ 0.5, -0.5,  0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
    cubeVertices[4] = (Vertex){-0.5,  0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[5] = (Vertex){-0.5, -0.5, -0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[6] = (Vertex){ 0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    cubeVertices[7] = (Vertex){ 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0};

    // Define triangles of the cube using counter-clockwise order
    cubeTriangles[0] = (Triangle){{cubeVertices[0], cubeVertices[2], cubeVertices[1]}};
    cubeTriangles[1] = (Triangle){{cubeVertices[1], cubeVertices[2], cubeVertices[3]}};

    cubeTriangles[2] = (Triangle){{cubeVertices[4], cubeVertices[5], cubeVertices[6]}};
    cubeTriangles[3] = (Triangle){{cubeVertices[5], cubeVertices[7], cubeVertices[6]}};

    cubeTriangles[4] = (Triangle){{cubeVertices[4], cubeVertices[0], cubeVertices[5]}};
    cubeTriangles[5] = (Triangle){{cubeVertices[5], cubeVertices[0], cubeVertices[1]}};

    cubeTriangles[6] = (Triangle){{cubeVertices[2], cubeVertices[6], cubeVertices[3]}};
    cubeTriangles[7] = (Triangle){{cubeVertices[3], cubeVertices[6], cubeVertices[7]}};

    cubeTriangles[8] = (Triangle){{cubeVertices[4], cubeVertices[6], cubeVertices[0]}};
    cubeTriangles[9] = (Triangle){{cubeVertices[6], cubeVertices[2], cubeVertices[0]}};

    cubeTriangles[10] = (Triangle){{cubeVertices[1], cubeVertices[3], cubeVertices[5]}};
    cubeTriangles[11] = (Triangle){{cubeVertices[3], cubeVertices[7], cubeVertices[5]}};

    // Calculate normals for each face
    for (int i = 0; i < 12; ++i) {
        calculateNormals(&cubeTriangles[i].verts[0], &cubeTriangles[i].verts[1], &cubeTriangles[i].verts[2]);
    }
}

void draw_mesh(Triangle tris[], int size) {
    for (int i = 0; i < size; ++i) {
        draw_simple_triangle(&tris[i]);
    }
}