#include <dream.h>
#include <stdio.h>
#include <time.h>


void checkGLError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %d\n", err);
    }
}


int main() {
	clock_t start = clock();
	DRMwindow* window;
	drmInit(&window);
	drmShowMouse(window, false);

	Camera cam;
	vec3 startPos = {0.0f, 0.0f, 5.0f};
	unsigned int shaderProgram;
	drmInitCamera(&cam, startPos);
	glfwSetWindowUserPointer(window, &cam);

	Texture tex;
	drmInitTEX("assets/grass.jpg", &tex, &shaderProgram);

	const short len = 25;

	Mesh cubes[len];
	drmInitShaders("shaders/vertex.glsl", "shaders/fragment.glsl", &shaderProgram);
	checkGLError();
	
	for (int i = 0; i < len; i++) {
		drmLoadOBJ("obj/grass.obj", &cubes[i]);
		for (int j = 0; j < cubes[i].vertsLen; j++) {
			cubes[i].verts[j].pos[1] += 3.0f;
			cubes[i].verts[j].pos[0] += 2.0f * i;
		}
		drmInitOBJ(&cubes[i]);
		cubes[i].tex = &tex;
	}




	float currentFrame, deltaTime = 0, lastFrame = 0;

	clock_t end = clock();
	double duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Duration: %.6f seconds\n", duration);
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drmProcessKeyboardInput(&cam, window, deltaTime);
		for (int i = 0; i < len; i++) {
			drmRenderOBJ(&shaderProgram, &cubes[i], &cam);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			printf("OpenGL error: %d\n", err);
		}
	}
	for (int i = 0; i < len; i++) {
		drmFreeOBJ(&cubes[i]);
	}
	glfwTerminate();
	return 0;
}


