#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

char* readFile(char* filePath, long* fileSize);
GLuint compileShader(GLenum type, const char* source);

#endif