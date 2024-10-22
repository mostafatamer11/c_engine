#version 330 core

layout (location = 0) in vec3 aPos;        // Position (vec3)
layout (location = 1) in vec3 aNormal;     // Normal (optional vec3)
layout (location = 2) in vec2 aTexCoord;   // Texture coordinates (optional vec2)

out vec3 FragPos;           // Pass the transformed position to the fragment shader
out vec3 Normal;            // Pass the normal vector to the fragment shader
out vec2 TexCoord;          // Pass texture coordinates (optional) to the fragment shader

uniform mat4 model;         // Model transformation matrix
uniform mat4 view;          // View transformation matrix
uniform mat4 projection;    // Projection matrix

void main()
{
    // Calculate the transformed position of the vertex in world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform the vertex position to clip space (projection * view * model * position)
    gl_Position = projection * view * vec4(FragPos, 1.0);

    // Pass the normal vector, transformed by the model matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass the texture coordinates (if applicable)
    TexCoord = aTexCoord;
}

