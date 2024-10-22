#ifndef SHADER_H
#define SHADER_H

void checkCompileErrors(unsigned int shader, const char* type);
void initShaders(const char* vertFile, const char* fragFile, unsigned int* shaderProgram);

#endif
