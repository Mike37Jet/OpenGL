#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord1;
in vec2 TexCoord2;
in vec2 TexCoord3;
in vec2 TexCoord4;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4; 

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform int useTexture;

void main()
{
    vec3 ambient, diffuse, specular;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    if (useTexture == 3) {
        ambient = light.ambient * texture(texture1, TexCoord1).rgb;
        diffuse = light.diffuse * diff * texture(texture1, TexCoord1).rgb;  
        specular = light.specular * spec * texture(texture1, TexCoord1).rgb;  
    }
    else if (useTexture == 2) {
        ambient = light.ambient * texture(texture2, TexCoord2).rgb;
        diffuse = light.diffuse * diff * texture(texture2, TexCoord2).rgb;  
        specular = light.specular * spec * texture(texture2, TexCoord2).rgb;  
    }
    else if (useTexture == 1) {
        ambient = light.ambient * texture(texture3, TexCoord3).rgb;
        diffuse = light.diffuse * diff * texture(texture3, TexCoord3).rgb;  
        specular = light.specular * spec * texture(texture3, TexCoord3).rgb;  
    }
    else {
        ambient = light.ambient * texture(texture4, TexCoord4).rgb;
        diffuse = light.diffuse * diff * texture(texture4, TexCoord4).rgb;  
        specular = light.specular * spec * texture(texture4, TexCoord4).rgb;  
    }
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
