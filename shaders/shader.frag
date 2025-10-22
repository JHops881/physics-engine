#version 330 core

in vec3 color;
in vec3 TexCoord;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, TexCoord) * vec4(color, 1.0f);
};