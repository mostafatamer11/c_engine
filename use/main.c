#include <engine.h>


int main() {
	GLFWwindow* window;
	initEng(&window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/


	Camera cam;
	vec3 startPos = {0.0f, 0.0f, 5.0f};
	initCamera(&cam, startPos);
	glfwSetWindowUserPointer(window, &cam);

	Vertex *vertices = NULL;
	Triangle *tris = NULL;
	int vertexCount = 0, indexCount = 0;
	loadOBJ("obj/obj.obj", &vertices, &vertexCount, &tris, &indexCount);
/*	Vertex vertices[] = {*/
/*    { {-1.000000, -1.000000,  1.000000}, {.normal = {-1.0000, -0.0000, -0.0000}, .texcoord = {0.750000, 0.333333}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { {-1.000000,  1.000000,  1.000000}, {.normal = {-1.0000, -0.0000, -0.0000}, .texcoord = {0.750000, 0.666666}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { {-1.000000, -1.000000, -1.000000}, {.normal = {-0.0000, -0.0000, -1.0000}, .texcoord = {0.500000, 0.666666}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { {-1.000000,  1.000000, -1.000000}, {.normal = {-0.0000, -0.0000, -1.0000}, .texcoord = {0.500000, 0.333333}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { { 1.000000, -1.000000,  1.000000}, {.normal = {1.0000, -0.0000, -0.0000}, .texcoord = {0.250000, 0.333333}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { { 1.000000,  1.000000,  1.000000}, {.normal = {1.0000, -0.0000, -0.0000}, .texcoord = {0.250000, 0.666666}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { { 1.000000, -1.000000, -1.000000}, {.normal = {1.0000, -0.0000, -0.0000}, .texcoord = {0.000000, 0.666666}}, VERTEX_NORMAL_TEXCOORD },*/
/*    { { 1.000000,  1.000000, -1.000000}, {.normal = {1.0000, -0.0000, -0.0000}, .texcoord = {0.000000, 0.333333}}, VERTEX_NORMAL_TEXCOORD }*/
/*};*/

/*	Triangle tris[] = {*/
/*    // Front face*/
/*    { { 0, 1, 2 } }, */
/*    { { 2, 1, 3 } },*/
/**/
/*    // Back face*/
/*    { { 4, 5, 6 } }, */
/*    { { 6, 5, 7 } },*/
/**/
/*    // Left face*/
/*    { { 0, 2, 4 } },*/
/*    { { 4, 2, 6 } },*/
/**/
/*    // Right face*/
/*    { { 1, 5, 3 } },*/
/*    { { 3, 5, 7 } },*/
/**/
/*    // Top face*/
/*    { { 1, 0, 5 } }, */
/*    { { 5, 0, 4 } },*/
/**/
/*    // Bottom face*/
/*    { { 3, 2, 7 } },*/
/*    { { 7, 2, 6 } }*/
/*};*/

	/*int vertexCount = 8;*/
	/*int indexCount = 12;*/


	unsigned int shaderProgram;
	initShaders("shaders/vertex.glsl", "shaders/fragment.glsl", &shaderProgram);

	unsigned int VBO, EBO, VAO;
	initOBJ(vertices, tris, vertexCount, indexCount*3, &VBO, &EBO, &VAO);
	for (int i = 0; i < vertexCount; ++i) {
    printf("Vertex %d: pos=(%f, %f, %f)\n", i, vertices[i].pos[0], vertices[i].pos[1], vertices[i].pos[2]);
}
for (int i = 0; i < indexCount; ++i) {
    printf("Triangle %d: indices=(%d, %d, %d)\n", i, tris[i].indices[0], tris[i].indices[1], tris[i].indices[2]);
}


	float currentFrame, deltaTime = 0, lastFrame = 0;
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput(&cam, window, deltaTime);

		renderOBJ(&shaderProgram, &VAO, &cam, indexCount * 3);
		glGetError();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	freeOBJ(vertices, tris);

	glfwTerminate();
	return 0;
}

