#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "draw.h"
#include "shader.h"
#include "camera.h"
#include "mmath.h"

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "GAME", NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW Window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize glad\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Vertex cubeVertices[8];
    Triangle cubeTriangles[12];

    createCubeMesh(cubeVertices, cubeTriangles);

    long fragmentShaderSize;
    long vertexShaderSize;

    char* fragmentShaderPath = "res/shaders/frag.glsl";
    char* vertexShaderPath = "res/shaders/vert.glsl";

    char* fragmentShaderSource = readFile(fragmentShaderPath, &fragmentShaderSize);
    char* vertexShaderSource = readFile(vertexShaderPath, &vertexShaderSize);

    if (!vertexShaderSource || !fragmentShaderSource) {
        fprintf(stderr, "Failed to load shader sources\n");
        return -1;
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader program linking failed: %s\n", infoLog);
        return -1;
    }

    glUseProgram(shaderProgram);

    int locRotation = glGetUniformLocation(shaderProgram, "rotation");
    int locProj = glGetUniformLocation(shaderProgram, "proj");

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float rotationAngle = 0.0f;
    vec3 rotationAxis = {1.0f, 1.0f, 1.0f};

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rotationAngle += 1.0f * deltaTime;

        mat4 rotationMatrix, projection;
        glm_rotate_make(rotationMatrix, rotationAngle, rotationAxis);

        glUniformMatrix4fv(locRotation, 1, GL_FALSE, (float*)rotationMatrix);

        glfwGetFramebufferSize(window, &width, &height);
        float fov = glm_rad(45.0f);
        float aspect = (float)width / (float)height;
        float near = 0.1f;
        float far = 100.0f;
        glm_perspective(fov, aspect, near, far, projection);
        glUniformMatrix4fv(locProj, 1, GL_FALSE, (float*)projection);

        vec4 v = {100.0f, 200.0f, 300.0f, 1.0f};
        vec4 c;

        glm_mat4_mulv(projection, v, c);

        printf("c: (%f, %f, %f)\r", c[0], c[1], c[2]);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int size = sizeof(cubeTriangles) / sizeof(Triangle);
        draw_mesh(cubeTriangles, size);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
