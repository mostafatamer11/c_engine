#include "engine.h"
#include <stdio.h>

    
void initShaders(const char* vertFile, const char* fragFile, unsigned int* shaderProgram) {
    // Open vertex and fragment shader files
    FILE* vertFileObj = fopen(vertFile, "r");
    FILE* fragFileObj = fopen(fragFile, "r");
    
    if (vertFileObj == NULL) {
        perror("Error opening vertex shader file");
        return;
    }
    if (fragFileObj == NULL) {
        perror("Error opening fragment shader file");
        fclose(vertFileObj); // Close already opened file
        return;
    }

    // Read vertex shader file size
    fseek(vertFileObj, 0, SEEK_END);
    long vertFileSize = ftell(vertFileObj);
    rewind(vertFileObj);

    // Read fragment shader file size
    fseek(fragFileObj, 0, SEEK_END);
    long fragFileSize = ftell(fragFileObj);
    rewind(fragFileObj);

    // Allocate memory for vertex and fragment shader sources
    char* vertBuffer = (char*)malloc(vertFileSize + 1);  // +1 for null terminator
    char* fragBuffer = (char*)malloc(fragFileSize + 1);  // +1 for null terminator

    if (vertBuffer == NULL || fragBuffer == NULL) {
        perror("Memory allocation failed");
        fclose(vertFileObj);
        fclose(fragFileObj);
        return;
    }

    // Read the shader files into buffers
    fread(vertBuffer, 1, vertFileSize, vertFileObj);
    vertBuffer[vertFileSize] = '\0';  // Null-terminate the vertex shader source

    fread(fragBuffer, 1, fragFileSize, fragFileObj);
    fragBuffer[fragFileSize] = '\0';  // Null-terminate the fragment shader source

    // Close the files as we no longer need them
    fclose(vertFileObj);
    fclose(fragFileObj);

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertBuffer, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragBuffer, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Create shader program and link shaders
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);
    checkCompileErrors(*shaderProgram, "PROGRAM");

    // Clean up by deleting shaders (they are no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Free allocated buffers
    free(vertBuffer);
    free(fragBuffer);
}

void checkCompileErrors(unsigned int shader, const char* type) {
    GLint success;
    GLchar infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}
