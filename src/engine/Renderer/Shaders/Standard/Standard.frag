#version 410

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

uniform sampler2D texSampler;

void main()
{
    // Funny hardcoded spotlight
    vec3 spotlightPos = vec3(0.0, 5.0, 0.0);
    vec3 spotlightDir = vec3(0.0, -1.0, 0.0);
    float cutoffAngle = cos(radians(90));
    vec3 ambientColor = vec3(0.1, 0.1, 0.1);
    vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
    vec3 specularColor = vec3(1.0, 1.0, 1.0);
    float shininess = 32.0;
    vec3 viewPos = vec3(0.0, 0.0, 10.0);

    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(spotlightPos - fragPosition);
    vec3 viewDir = normalize(viewPos - fragPosition);

    float theta = dot(lightDir, normalize(-spotlightDir));
    if (theta > cutoffAngle)
    {
        vec3 ambient = ambientColor * fragColor;

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * diffuseColor * fragColor;

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularColor * spec;

        vec3 finalColor = ambient + diffuse + specular;
        outColor = vec4(finalColor, 1.0) * texture(texSampler, fragTexCoord);
    }
    else
    {
        outColor = vec4(ambientColor * fragColor, 1.0) * texture(texSampler, fragTexCoord);
    }
}