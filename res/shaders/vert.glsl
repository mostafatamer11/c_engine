#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 Normal;  
out vec3 Color;

uniform mat4 rotation;
uniform mat4 proj;

void main() {
    Color = color;
    gl_Position = vec4(position, 1.0) * rotation;
}
