#include "shader.h"


// Function to read the content of a file
#include <stdio.h>
#include <stdlib.h>

char* readFile(char* filename, long* fileSize) {
    FILE* file;
    char* content;

    file = fopen(filename, "rb");

    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    content = (char*)malloc(*fileSize);

    if (content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(content, 1, *fileSize, file);

    content[*fileSize] = '\0';

    fclose(file);
    return content;
}



GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader compilation failed: %s\n", infoLog);
        return 0;
    }

    return shader;
}