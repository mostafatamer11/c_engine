#include "dream.h"
#include <GLFW/glfw3.h>


void readFile(const char* filename) {
	FILE* file = fopen(filename, "r");

	if (file == NULL) {
		perror("Error opening file");
		return;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);  // Get the file size
	rewind(file);  // Reset file pointer to the beginning

	char buffer[fileSize + 1];  // fileSize + 1 for null-terminator

	size_t bytesRead = fread(buffer, 1, fileSize, file);
	buffer[bytesRead] = '\0';  // Null-terminate the buffer

	printf("%s\n", buffer);

	fclose(file);
}

void drmResizeCallback(GLFWwindow *window, int width, int height);

int drmInit(GLFWwindow **window) {
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(800, 600, "3D Rotating Cube", NULL, NULL);
	if (!(*window)) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(*window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glfwSetCursorPosCallback(*window, drmMouseCallback);
	stbi_set_flip_vertically_on_load(true);
	glfwSetFramebufferSizeCallback(*window, drmResizeCallback);
	glfwSwapInterval(1);
	return 0;
}

void drmShowMouse(DRMwindow* window, bool i) {
	if (i) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void drmResizeCallback(GLFWwindow *window, int width, int height) {
	const float originalAspectRatio = 16.0f / 9.0f;

    // Calculate the aspect ratio of the current window
    float windowAspectRatio = (float)width / (float)height;

    int viewportWidth, viewportHeight;
    
    // Adjust dimensions based on aspect ratios
    if (windowAspectRatio > originalAspectRatio) {
        // Window is wider than the desired aspect ratio
        viewportHeight = height;
        viewportWidth = height * originalAspectRatio; // Calculate width based on height
    } else {
        // Window is taller than the desired aspect ratio
        viewportWidth = width;
        viewportHeight = width / originalAspectRatio; // Calculate height based on width
    }

    // Center the viewport
    int xOffset = (width - viewportWidth) / 2;
    int yOffset = (height - viewportHeight) / 2;

    // Set the viewport to fill the area without borders
    glViewport(xOffset, yOffset, viewportWidth, viewportHeight);
}

