#version 330 core
out vec4 FragColor;

in vec2 TexCoord1;
in vec2 TexCoord2;
in vec2 TexCoord3;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int useTexture1;

void main()
{
    if (useTexture1 == 2)
        FragColor = texture(texture1, TexCoord1);
    else if (useTexture1 == 1)
        FragColor = texture(texture2, TexCoord2);
    else
        FragColor = texture(texture3, TexCoord3);
}