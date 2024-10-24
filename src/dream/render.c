#include "dream.h"

void initMesh(Vertex* vertices, Triangle* indices, size_t vertexCount, size_t indexCount, unsigned int *VBO, unsigned int *EBO, unsigned int *VAO) {
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    // Step 1: Create and bind the VBO
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // Step 2: Create and bind the EBO
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Step 3: Define the vertex attribute layout
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
    glEnableVertexAttribArray(1);

    // Step 4: Unbind the VAO
    glBindVertexArray(0);

    // Enable depth testing for correct rendering of 3D objects
    glEnable(GL_DEPTH_TEST);
}

void drmInitOBJ(Mesh* mesh) {
    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &mesh->VAO);
    glBindVertexArray(mesh->VAO);

    // Step 1: Create and bind the Vertex Buffer Object (VBO)
    glGenBuffers(1, &mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertsLen * sizeof(Vertex), mesh->verts, GL_STATIC_DRAW);

    // Step 2: Create and bind the Element Buffer Object (EBO)
    glGenBuffers(1, &mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->trisLen * 3 * sizeof(unsigned int), mesh->tris, GL_STATIC_DRAW);

    // Step 3: Define the vertex attribute layout
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1) - check if VertexType allows normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Texcoord attribute (location = 2) - if the VertexType has texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // Step 4: Unbind the VAO (safe state)
    glBindVertexArray(0);

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
}

void drmRenderOBJ(unsigned int *shaderProgram, Mesh* mesh, Camera *camera) {
    // Clear the screen and depth buffer

    // Use the shader program
    glUseProgram(*shaderProgram);

    // Prepare model, view, and projection matrices
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view;
    mat4 projection;
    // Camera's view matrix
    drmGetViewMatrix(camera, view);

    // Projection matrix
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    // Safety checks for uniform locations and apply them if they exist
    GLint modelLoc = glGetUniformLocation(*shaderProgram, "model");
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);
    
    GLint viewLoc = glGetUniformLocation(*shaderProgram, "view");
    if (viewLoc != -1) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);

    GLint projLoc = glGetUniformLocation(*shaderProgram, "projection");
    if (projLoc != -1) glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat*)projection);

    // Bind the VAO and draw the elements (triangles)
    glBindVertexArray(mesh->VAO);
	GLint lightPosLoc = glGetUniformLocation(*shaderProgram, "lightPos");
	GLint lightColorLoc = glGetUniformLocation(*shaderProgram, "lightColor");
	GLint objectColorLoc = glGetUniformLocation(*shaderProgram, "objectColor");
	GLint useTextureLoc = glGetUniformLocation(*shaderProgram, "useTexture");
	glBindTexture(GL_TEXTURE_2D, mesh->tex->TEX);
	GLint textureLoc = glGetUniformLocation(*shaderProgram, "texture1");

	glUniform3f(lightPosLoc, 0.0f, 10.0f, 10.0f);        // Example light position
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);      // White light
	glUniform3f(objectColorLoc, 1.0f, 1.0f, 0.31f);    // Example object color
	glUniform1i(useTextureLoc, GL_TRUE);
	glUniform1i(textureLoc, 0);
    glDrawElements(GL_TRIANGLES, mesh->trisLen * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


