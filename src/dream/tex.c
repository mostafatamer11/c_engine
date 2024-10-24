#include "dream.h"

void drmInitTEX(const char* filePath, Texture* tex, unsigned int* shader) {
    stbi_set_flip_vertically_on_load(true);
    tex->bytes = stbi_load(filePath, &tex->width, &tex->height, &tex->colNum, 0);
    if (!tex->bytes) {
        printf("Failed to load texture: %s\n", filePath);
        return;
    }
    glGenTextures(1, &tex->TEX);
    glBindTexture(GL_TEXTURE_2D, tex->TEX);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load texture
    if (tex->colNum == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->bytes);
    } else if (tex->colNum == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->bytes);
    } else {
        printf("Unsupported number of color channels: %d\n", tex->colNum);
        stbi_image_free(tex->bytes);
        return;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex->bytes);

    // Bind shader and set uniform
    /*glUseProgram(*shader);*/
    /*int textureLoc = glGetUniformLocation(*shader, "texture1");*/
    /*if (textureLoc == -1) {*/
    /*    printf("Warning: uniform 'texture1' not found in shader program\n");*/
    /*} else {*/
    /*    glUniform1i(textureLoc, 0);  // Assuming the texture unit is 0*/
    /*}*/
    /*int useTextureLoc = glGetUniformLocation(*shader, "useTexture");*/
    /*if (useTextureLoc == -1) {*/
    /*    printf("Warning: uniform 'useTexture' not found in shader program\n");*/
    /*} else {*/
    /*    glUniform1i(useTextureLoc, GL_TRUE);*/
    /*}*/

    // Unbind the texture to avoid accidental usage later
    glBindTexture(GL_TEXTURE_2D, 0);
}


