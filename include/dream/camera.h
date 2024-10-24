#ifndef DRM_CAM_H
#define DRM_CAM_H

#include "dream.h"

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

void drmMouseCallback(DRMwindow* window, double xpos, double ypos);
void drmInitCamera(Camera* camera, vec3 startPosition);
void drmUpdateCameraVectors(Camera* camera);
void drmProcessKeyboardInput(Camera* camera, DRMwindow* window, float deltaTime);
void drmProcessMouseInput(Camera* camera, float xOffset, float yOffset);
void drmGetViewMatrix(Camera* camera, mat4 view);

#endif

