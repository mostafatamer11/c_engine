#include "dream.h"

void drmInitTEX(const char* filePath, Mesh* mesh, unsigned int* shader) {
    stbi_set_flip_vertically_on_load(true);
    mesh->tex.bytes = stbi_load(filePath, &mesh->tex.width, &mesh->tex.height, &mesh->tex.colNum, 0);
    if (!mesh->tex.bytes) {
        printf("Failed to load texture: %s\n", filePath);
        return;
    }

    glGenTextures(1, &mesh->TEX);
    glBindTexture(GL_TEXTURE_2D, mesh->TEX);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load texture
    if (mesh->tex.colNum == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh->tex.width, mesh->tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mesh->tex.bytes);
    } else if (mesh->tex.colNum == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh->tex.width, mesh->tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, mesh->tex.bytes);
    } else {
        printf("Unsupported number of color channels: %d\n", mesh->tex.colNum);
        stbi_image_free(mesh->tex.bytes);
        return;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(mesh->tex.bytes);

    // Bind shader and set uniform
    glUseProgram(*shader);
    
    int textureLoc = glGetUniformLocation(*shader, "texture1");
    if (textureLoc == -1) {
        printf("Warning: uniform 'texture1' not found in shader program\n");
    } else {
        glUniform1i(textureLoc, 0);  // Assuming the texture unit is 0
    }

    int useTextureLoc = glGetUniformLocation(*shader, "useTexture");
    if (useTextureLoc == -1) {
        printf("Warning: uniform 'useTexture' not found in shader program\n");
    } else {
        glUniform1i(useTextureLoc, GL_TRUE);
    }

    // Unbind the texture to avoid accidental usage later
    glBindTexture(GL_TEXTURE_2D, 0);

}


