#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int textureSelector;

void main()
{
    if (textureSelector == 0) {
        FragColor = texture(texture1, TexCoord); // Usa texture1
    } else {
        FragColor = texture(texture2, TexCoord); // Usa texture2
    }
}