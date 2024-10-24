#include <dream.h>

int main() {
	DRMwindow* window;
	drmInit(&window);
	drmShowMouse(window, false);

	Camera cam;
	vec3 startPos = {0.0f, 0.0f, 5.0f};
	drmInitCamera(&cam, startPos);
	glfwSetWindowUserPointer(window, &cam);

	Mesh cube;
	drmLoadOBJ("obj/grass.obj", &cube);
	drmInitOBJ(&cube);

	unsigned int shaderProgram;
	drmInitShaders("shaders/vertex.glsl", "shaders/fragment.glsl", &shaderProgram);
	drmInitTEX("assets/grass.jpg", &cube, &shaderProgram);

	float currentFrame, deltaTime = 0, lastFrame = 0;

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drmProcessKeyboardInput(&cam, window, deltaTime);
		drmRenderOBJ(&shaderProgram, &cube, &cam);

		glfwSwapBuffers(window);
		glfwPollEvents();
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			printf("OpenGL error: %d\n", err);
		}
	}
	drmFreeOBJ(&cube);
	glfwTerminate();
	return 0;
}


