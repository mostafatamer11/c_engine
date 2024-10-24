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
	initShaders("shaders/vertex.glsl", "shaders/fragment.glsl", &shaderProgram);

	Texture tex;
	unsigned int tex_id;
	initTEX("assets/grass.jpg", &tex, &tex_id, &cube.VAO, &shaderProgram);

	float currentFrame, deltaTime = 0, lastFrame = 0;

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, tex_id);

		drmProcessKeyboardInput(&cam, window, deltaTime);
		drmRenderOBJ(&shaderProgram, &cube, &cam);

		glBindVertexArray(cube.VAO);
		glDrawElements(GL_TRIANGLES, cube.trisLen * 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			printf("OpenGL error: %d\n", err);
		}
		printf("FPS: %f\r", 1/deltaTime);
	}
	drmFreeOBJ(&cube);
	glfwTerminate();
	printf("\n");
	return 0;
}


