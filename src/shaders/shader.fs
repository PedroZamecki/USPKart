//#version 330 core
//out vec4 FragColor;

//void main()
//{
//	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//}
//--------------------------------------------
//#version 330 core
//out vec4 FragColor;
//in vec3 ourColor;

//void main()
//{
//	FragColor = vec4(ourColor, 1.0);
//}
//--------------------------------------------

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D ourTexture;
uniform vec3 lightMin;

void main()
{
//	vec3 objectColor = vec3(1.0, 0.5, 0.31);
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = max(ambient + diffuse + specular, lightMin) * ourColor;
	FragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);

//	FragColor = vec4(result, 1.0);
//	FragColor = texture(ourTexture, TexCoord);
}