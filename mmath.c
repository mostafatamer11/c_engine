#include "mmath.h"

void createRotationMatrix(float angle, vec3 axis, mat4 model) {
    glm_mat4_identity(model);
    glm_rotate(model, angle, axis);
}