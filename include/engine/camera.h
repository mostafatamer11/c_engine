#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GL/gl.h>

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
} Camera;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void initCamera(Camera* camera, vec3 startPosition);
void updateCameraVectors(Camera* camera);
void processKeyboardInput(Camera* camera, GLFWwindow* window, float deltaTime);
void processMouseInput(Camera* camera, float xOffset, float yOffset);
void getViewMatrix(Camera* camera, mat4 view);

#endif

