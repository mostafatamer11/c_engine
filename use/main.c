#include <engine.h>

int main() {
    GLFWwindow* window;
    initEng(&window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    stbi_set_flip_vertically_on_load(true);

    Camera cam;
    vec3 startPos = {0.0f, 0.0f, 5.0f};
    initCamera(&cam, startPos);
    glfwSetWindowUserPointer(window, &cam);

    Vertex *vertices = NULL;
    Triangle *tris = NULL;
    int vertexCount = 0, indexCount = 0;
    loadOBJ("obj/grass.obj", &vertices, &vertexCount, &tris, &indexCount);

    unsigned int shaderProgram;
    initShaders("shaders/vertex.glsl", "shaders/fragment.glsl", &shaderProgram);

    unsigned int VBO, EBO, VAO;
    initOBJ(vertices, tris, vertexCount, indexCount * 3, &VBO, &EBO, &VAO);

    for (int i = 0; i < vertexCount; ++i) {
        printf("Vertex %d: texcoord=(%f, %f), type=%d\n", i, vertices[i].texcoord[0], vertices[i].texcoord[1], vertices[i].type == VERTEX_NORMAL_TEXCOORD);
    }
    for (int i = 0; i < indexCount; ++i) {
        printf("Triangle %d: indices=(%d, %d, %d)\n", i, tris[i].indices[0], tris[i].indices[1], tris[i].indices[2]);
    }

    Texture tex;
    unsigned int tex_id;
    initTEX("assets/grass.jpg", &tex, &tex_id, &VAO, &shaderProgram);

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

        processKeyboardInput(&cam, window, deltaTime);
        renderOBJ(&shaderProgram, &VAO, &cam, indexCount * 3);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR) {
			    printf("OpenGL error: %d\n", err);
		}
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &tex_id);
	freeOBJ(vertices, tris);
	glfwTerminate();
	return 0;
}


