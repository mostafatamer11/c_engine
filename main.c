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

    Camera camera;
    createCamera(&camera, (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f});

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

    GLuint modelViewLoc = glGetUniformLocation(shaderProgram, "modelView");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float rotationAngle = 0.0f;
    vec3 rotationAxis = {0.0f, 1.0f, 1.0f};

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rotationAngle += 1.0f * deltaTime;

        mat4 rotationMatrix;
        glm_rotate_make(rotationMatrix, rotationAngle, rotationAxis);

        vec3 newCameraPos;
        mat4 view;
        glm_vec3_scale(camera.front, 0.0f, newCameraPos);
        glm_vec3_add(camera.position, newCameraPos, newCameraPos);
        glm_lookat(newCameraPos, newCameraPos, camera.up, view);

        mat4 modelView;
        mat4 projection;

        glm_mat4_identity(modelView);
        glm_mul(view, rotationMatrix, modelView);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

        float fov = glm_rad(45.0f);
        float aspect = (float)640 / (float)480;
        float near = 0.1f;
        float far = 100.0f;
        glm_perspective(fov, aspect, near, far, projection);

        glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, (float*)modelView);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

        glUniform3f(lightPosLoc, 1.0f, 2.0f, 0.0f);
        glUniform3f(viewPosLoc, 0.0f, 0.0f, 3.0f);
        glUniform3f(lightColorLoc, 0.0f, 0.0f, 1.0f);
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.5f);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int size = sizeof(cubeTriangles) / sizeof(Triangle);
        draw_mesh(cubeTriangles, size);

        updateCamera(&camera, window, deltaTime, 0.05f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
