#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 lightFragPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

float shadowCalculation(vec4 fragPosLightSpace) {
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   projCoords = projCoords * 0.5 + 0.5;
   if (projCoords.z > 1.f) return 0.f;
   float closestDepth = texture(shadowMap, projCoords.xy).r;
   float currentDepth = projCoords.z;

   vec3 normal = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float bias = max(0.05 * (1 - dot(normal, -lightDir)), 0.005);
   float shadow = 0.f;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   for (int x = -1; x <= 1; x++) {
       for (int y = -1; y <= 1; y++) {
           float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
           shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
       }
   }
   shadow /= 9.0;
   return shadow;
}

void main() {
   vec3 color = texture(diffuseTexture, TexCoords).rgb;
   vec3 normal = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 halfwayDir = normalize(viewDir + lightDir);
   vec3 lightColor = vec3(0.3);

   vec3 ambient = 0.3 * lightColor;
   float diff = max(dot(lightDir, normal), 0.0);
   vec3 diffuse = lightColor * diff;
   float spec = max(dot(halfwayDir, normal), 0);
   vec3 specular = spec * lightColor;
   float shadow = shadowCalculation(lightFragPos);

   vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
   FragColor = vec4(lighting, 1);
}