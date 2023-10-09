#version 430

precision highp float;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform sampler2D tex;

out vec4 FragColor;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;
	vec3 cameraDir = normalize(cameraPosition - FragPosition);
	vec3 reflectionDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(cameraDir, reflectionDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	vec4 texColor = texture(tex, TexCoord);
	FragColor = texColor * vec4(result, 1.0);
}