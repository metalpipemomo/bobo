#version 410

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

uniform sampler2D texSampler;
uniform vec3 cameraPos;
uniform float shininess;

struct Spotlight
{
    vec3 position;
    vec3 direction;
    float cutoffAngle;
    vec3 baseColor;
};

struct Pointlight
{
    vec3 position;
    vec3 baseColor;
};

uniform int numSpotlights;
uniform Spotlight spotlights[100];

uniform int numPointlights;
uniform Pointlight pointlights[100];

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 cameraDir = normalize(cameraPos - fragPosition);

    vec3 finalColor = vec3(0.0);
    for (int i = 0; i < numSpotlights; i++)
    {
        Spotlight light = spotlights[i];

        vec3 lightDir = normalize(light.position - fragPosition);
        float theta = dot(lightDir, normalize(-light.direction));
        float cutoffAngle = cos(radians(light.cutoffAngle));

        vec3 spotlightAmbient = light.baseColor * 0.3;
        vec3 spotlightEffect = vec3(0.0);

        if (theta > cutoffAngle)
        {
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * light.baseColor * fragColor;
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(cameraDir, reflectDir), 0.0), shininess);
            vec3 specular = light.baseColor * spec;
            spotlightEffect = diffuse + specular;
        }

        finalColor += spotlightAmbient * fragColor + spotlightEffect;
    }

    for (int i = 0; i < numPointlights; i++)
    {
        Pointlight light = pointlights[i];

        vec3 lightDir = normalize(light.position - fragPosition);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * light.baseColor * fragColor;
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(cameraDir, reflectDir), 0.0), shininess);
        vec3 specular = light.baseColor * spec;

        vec3 ambient = light.baseColor * 0.3;
        finalColor += ambient * fragColor + diffuse + specular;
    }

    outColor = vec4(finalColor, 1.0) * texture(texSampler, fragTexCoord);
}