#include "engine.h"

void processMouseInput(Camera* camera, float xOffset, float yOffset) {

    xOffset *= camera->mouseSensitivity;
    yOffset *= camera->mouseSensitivity;
    
    camera->yaw += xOffset;
    camera->pitch += yOffset;


    // Limit pitch to avoid gimbal lock
    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    // Update camera vectors after changing pitch/yaw
    updateCameraVectors(camera);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (camera == NULL) {
        fprintf(stderr, "Camera is NULL\n");
        return;
    }

    static float lastX = 400, lastY = 300;
    static int firstMouse = 1;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;


    processMouseInput(camera, xOffset, yOffset);
}


void initCamera(Camera* camera, vec3 startPosition) {
    glm_vec3_copy(startPosition, camera->position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->worldUp);

    camera->yaw = -90.0f;  // Point along the negative Z-axis
    camera->pitch = 0.0f;
    camera->movementSpeed = 2.5f;
    camera->mouseSensitivity = 0.75f;
    camera->zoom = 45.0f;
    updateCameraVectors(camera);
}

void updateCameraVectors(Camera* camera) {
    // Calculate the new front vector
    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(front);
    glm_vec3_copy(front, camera->front);

    // Recalculate right and up vectors
    vec3 right;
    glm_cross(camera->front, camera->worldUp, right); // Right vector
    glm_normalize(right);
    glm_vec3_copy(right, camera->right);

    vec3 up;
    glm_cross(camera->right, camera->front, up); // Up vector
    glm_normalize(up);
    glm_vec3_copy(up, camera->up);
}

void processKeyboardInput(Camera* camera, GLFWwindow* window, float deltaTime) {
    float velocity = camera->movementSpeed * deltaTime;
    vec3 temp;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {  // Forward
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {  // Backward
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {  // Left
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {  // Right
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {  // Upward
        glm_vec3_scale(camera->worldUp, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {  // Downward
        glm_vec3_scale(camera->worldUp, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }
}

void getViewMatrix(Camera* camera, mat4 view) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);  // Target point
    glm_lookat(camera->position, center, camera->up, view);
}

