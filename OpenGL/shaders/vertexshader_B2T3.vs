#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord1;
layout (location = 2) in vec2 aTexCoord2;
layout (location = 3) in vec2 aTexCoord3;

out vec2 TexCoord1;
out vec2 TexCoord2;
out vec2 TexCoord3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord1 = aTexCoord1;
    TexCoord2 = aTexCoord2;
    TexCoord3 = aTexCoord3;
}

