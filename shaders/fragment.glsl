#version 330 core

in vec3 FragPos;            // The fragment's position in world space
in vec3 Normal;             // The fragment's normal vector
in vec2 TexCoord;           // Texture coordinates (if applicable)

out vec4 FragColor;         // Output color of the fragment

uniform vec3 lightPos;      // Directional light position
uniform vec3 lightColor;    // Color of the light
uniform vec3 objectColor;   // Base color of the object (if no textures are used)
uniform sampler2D texture1; // Optional texture sampler (if using textures)

uniform bool useTexture;    // Whether to use texture mapping

void main()
{
    // Ambient lighting (weak baseline lighting)
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting (directional lighting)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine ambient and diffuse lighting
    vec3 lighting = ambient + diffuse;

    // Determine the final color
    if (useTexture) {
        // Sample the texture at the provided coordinates
        vec4 texColor = texture(texture1, TexCoord);
        FragColor = texColor * vec4(lighting, 1.0);  // Apply lighting to texture color
    } else {
        // Use objectColor if no texture is used
        FragColor = vec4(lighting * objectColor, 1.0); // Apply lighting to object color
    }
}

