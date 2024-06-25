#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform vec3 colorOffset;
uniform bool colorOrigin;

void main()
{
    if (colorOrigin)
		FragColor = vec4(ourColor, 1.0f);
	else
    FragColor = vec4(ourColor + colorOffset, 1.0f);
}