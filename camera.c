#include "camera.h"

void createCamera(Camera* camera, vec3 position, vec3 front, vec3 up) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(front, camera->front);
    glm_vec3_copy(up, camera->up);
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
}

void updateCamera(Camera* camera, GLFWwindow* window, float deltaTime, float sensitivity) {
    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glm_vec3_scale(camera->front, cameraSpeed, camera->front);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glm_vec3_scale(camera->front, -cameraSpeed, camera->front);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera->front, camera->up, right);
        glm_vec3_scale(right, -cameraSpeed, right);
        glm_vec3_add(camera->position, right, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera->front, camera->up, right);
        glm_vec3_scale(right, cameraSpeed, right);
        glm_vec3_add(camera->position, right, camera->position);
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float xOffset = (float)mouseX * sensitivity;
    float yOffset = (float)mouseY * sensitivity;

    camera->yaw += xOffset;
    camera->pitch += yOffset;

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize_to(front, camera->front);
}
