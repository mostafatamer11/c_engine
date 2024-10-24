#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stb_image.h>

#include "dream/main.h"
#include "dream/tex.h"
#include "dream/obj.h"
#include "dream/render.h"
#include "dream/shader.h"
#include "dream/camera.h"
#include "dream/geometry.h"


typedef GLFWwindow DRMwindow;

#define DRM_MOUSE_HIDE GLFW_CURSOR_DISABLED
#define DRM_MOUSE_SHOW GLFW_CURSOR_ENABLED


