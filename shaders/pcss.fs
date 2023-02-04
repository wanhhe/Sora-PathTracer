#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 lightFragPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

#define NUM_SAMPLES 100
#define PI2 6.283185307179586
#define PI 3.141592653589793
#define NUM_RINGS 10

vec2 poissonDisk[NUM_SAMPLES];

highp float rand_2to1(vec2 uv) {
   const highp float a = 12.9898, b = 78.233, c = 43758.5453;
   highp float dt = dot(uv.xy, vec2(a, b));
   highp float sn = mod( dt, PI );
   return fract(sin(sn) * c);
}

void poissonDiskSamples(const in vec2 randomSeed) {
   float ANGLE_STEP = PI2 * float(NUM_RINGS)/float( NUM_SAMPLES);
   float INV_NUM_SAMPLES = 1.0 / float(NUM_SAMPLES);
   float angle = rand_2to1(randomSeed) * PI2;
   float radius = INV_NUM_SAMPLES;
   float radiusStep = radius;
   for (int i = 0; i < NUM_SAMPLES; i++) {
       poissonDisk[i] = vec2(cos(angle), sin(angle)) * pow( radius, 0.75 );
       radius += radiusStep;
       angle += ANGLE_STEP;
   }
}

float pcf(vec3 projCoords, int r) {
   if (projCoords.z > 1) return 0.f;
   float cloestDepth = texture(shadowMap, projCoords.xy).r;
   float currentDepth = projCoords.z;
   float bias = max(0.008 * (1 - dot(normalize(Normal), normalize(FragPos - lightPos))), 0.004);
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   float shadow = 0.0;

   poissonDiskSamples(projCoords.xy);

   for (int i = 0; i < NUM_SAMPLES; i++) {
       float pcfDepth = texture(shadowMap, projCoords.xy + r * poissonDisk[i] * texelSize).r;
       shadow += currentDepth - bias > pcfDepth ? 1.f : 0.f;
   }

   shadow /= float(NUM_SAMPLES);
   return shadow;
}

float averageBlockerDepth(vec3 projCoords, vec2 texelSize) {
   float blockerZ = 0.0;
   int count = 0;
   int r = 5;

   poissonDiskSamples(projCoords.xy+vec2(0.1314,0.351));

   for (int i = 0; i < NUM_SAMPLES; i++) {
       float depth = texture(shadowMap, projCoords.xy + r * poissonDisk[i] * texelSize).r;
       if (depth < projCoords.z) {
           count++;
           blockerZ += depth;
       }
   }

   if (count == 0 || count == (2*r+1)*(2*r+1)) return 1.0f;
   return blockerZ / count;
}

float pcss(vec4 lightFragPos) {
   vec3 projCoords = lightFragPos.xyz / lightFragPos.w;
   projCoords = projCoords * 0.5 + 0.5;
   const float weightOfLight = 10.0;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   float averBlocker = averageBlockerDepth(projCoords, texelSize);
   float penumbra = (projCoords.z - averBlocker) * weightOfLight / averBlocker;
   float visiablity = pcf(projCoords, int(penumbra));
   return visiablity;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 color = texture(diffuseTexture, TexCoords).rgb;

    vec3 lightColor = vec3(0.3);
    vec3 ambient = 0.3 * lightColor;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    float spec = max(dot(halfwayDir, normal), 0);
    vec3 specular = spec * lightColor;
    float shadow = pcss(lightFragPos);
    
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting, 1.0);
}