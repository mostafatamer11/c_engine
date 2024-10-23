#ifndef TEX_H
#define TEX_H


typedef struct {
	int width, height, colNum;
	unsigned char* bytes;
} Texture;

void initTEX(const char* filePath, Texture* tex, unsigned int* gl_tex, unsigned int* VAO, unsigned int* shader);

#endif
