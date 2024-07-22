#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord1;
layout (location = 3) in vec2 aTexCoord2;
layout (location = 4) in vec2 aTexCoord3;
layout (location = 5) in vec2 aTexCoord4;

out vec3 FragPos; 
out vec3 Normal;
out vec2 TexCoord1;
out vec2 TexCoord2;
out vec2 TexCoord3;
out vec2 TexCoord4;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    TexCoord1 = aTexCoord1;
    TexCoord2 = aTexCoord2;
    TexCoord3 = aTexCoord3;
    TexCoord4 = aTexCoord4;

    Normal = mat3(transpose(inverse(model))) * aNormal; 
}

