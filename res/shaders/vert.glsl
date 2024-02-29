#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 FragPos;  
out vec3 Normal;  
out vec3 Color;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
    FragPos = vec3(modelView * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(modelView))) * normal;
    Color = color;

    gl_Position = projection * vec4(FragPos, 1.0);
}
