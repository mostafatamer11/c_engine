#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    float yaw;
    float pitch;
} Camera;

void updateCamera(Camera* camera, GLFWwindow* window, float deltaTime, float sensitivity);
void createCamera(Camera* camera, vec3 position, vec3 front, vec3 up);

#endif