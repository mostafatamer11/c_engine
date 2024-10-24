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

