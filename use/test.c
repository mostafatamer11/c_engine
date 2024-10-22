#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <string.h>


void loadOBJ(const char *fileName, float **vertices, unsigned int **indices, size_t *vertexCount, size_t *indexCount) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Failed to open file: %s\n", fileName);
        exit(1);
    }

    *vertexCount = 0;
    *indexCount = 0;
    size_t vertexCapacity = 1024;
    size_t indexCapacity = 1024;
    *vertices = malloc(vertexCapacity * 3 * sizeof(float));
    *indices = malloc(indexCapacity * sizeof(unsigned int));

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            if (*vertexCount >= vertexCapacity) {
                vertexCapacity *= 2;
                *vertices = realloc(*vertices, vertexCapacity * 3 * sizeof(float));
            }
            sscanf(line, "v %f %f %f", &(*vertices)[(*vertexCount) * 3], &(*vertices)[(*vertexCount) * 3 + 1], &(*vertices)[(*vertexCount) * 3 + 2]);
            (*vertexCount)++;
        } else if (strncmp(line, "f ", 2) == 0) {
            if (*indexCount >= indexCapacity) {
                indexCapacity *= 2;
                *indices = realloc(*indices, indexCapacity * sizeof(unsigned int));
            }
            unsigned int vertexIndex[3];
            sscanf(line, "f %u %u %u", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
            (*indices)[(*indexCount)++] = vertexIndex[0] - 1;
            (*indices)[(*indexCount)++] = vertexIndex[1] - 1;
            (*indices)[(*indexCount)++] = vertexIndex[2] - 1;
        }
    }
    fclose(file);
}



// Shader sources
const char* vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\0";

// Global variables
GLuint shaderProgram;
GLuint VAO, VBO, EBO;
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;

void checkCompileErrors(GLuint shader, char* type);


void initOpenGL() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	float *vertices_ = NULL;
	unsigned int *indices_ = NULL;
   	size_t vertexCount, indexCount;
	loadOBJ("obj/obj.obj", &vertices_, &indices_, &vertexCount, &indexCount);


    float vertices[] = {
        // Positions        // Colors
        -1.0f, -1.0f,  1.0f,  // Front face
    	1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,  // Back face
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3,  // Front face
        4, 5, 6, 4, 6, 7,  // Back face
        0, 1, 5, 0, 5, 4,  // Bottom face
        3, 2, 6, 3, 6, 7,  // Top face
        0, 4, 7, 0, 7, 3,  // Left face
        1, 5, 6, 1, 6, 2   // Right face
    };

    // Setup VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    // Model matrix (rotation)
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(model, glm_rad(angleX), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(model, glm_rad(angleY), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(model, glm_rad(angleZ), (vec3){0.0f, 0.0f, 1.0f});

    // View matrix (camera)
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){0.0f, 0.0f, -5.0f});

    // Projection matrix (perspective)
    mat4 projection;
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    // Send matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, (const GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, (const GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, (const GLfloat*)projection);

    // Draw cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void timer() {
    angleX += 1.0f;
    angleY += 1.0f;
    angleZ += 1.0f;
    if (angleX > 360.0f) angleX -= 360.0f;
    if (angleY > 360.0f) angleY -= 360.0f;
    if (angleZ > 360.0f) angleZ -= 360.0f;
    display();
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Rotating 3D Cube with GLFW", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    	printf("Failed to initialize GLAD\n");
	    return -1;
	}

	// Initialize OpenGL settings
	initOpenGL();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
    	// Process input
	    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	    glfwSetWindowShouldClose(window, true);

	    	// Render
    	display();

	    // Swap buffers
    	glfwSwapBuffers(window);

	    // Poll events
    	glfwPollEvents();

	    // Timer for rotation
    	timer();
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void checkCompileErrors(GLuint shader, char* type) {
    GLint success;
    GLchar infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}
