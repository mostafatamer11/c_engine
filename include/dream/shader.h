#ifndef DRM_SHADER_H
#define DRM_SHADER_H

void checkCompileErrors(unsigned int shader, const char* type);
void initShaders(const char* vertFile, const char* fragFile, unsigned int* shaderProgram);

#endif
