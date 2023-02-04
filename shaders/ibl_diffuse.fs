#version 330 core
out vec4 FragColor;
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform float ao;

uniform samplerCube irradianceMap;

uniform vec3 lightPosition[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;

const float PI = 3.15159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
   return F0 +(1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
   float alpha = roughness * roughness;
   float alpha2 = alpha * alpha;

   float NdotH = max(dot(N, H), 0.0);
   float NdotH2 = NdotH * NdotH;

   float nom = alpha2;
   float denom = (NdotH2 * (alpha2 - 1.0) + 1.0);

   denom = PI * denom * denom;
   return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
   float r = roughness + 1.0;
   float k = (r * r) /8.0;
   float nom = NdotV;

   float denom = NdotV * (1.0 - k) + k;
   return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
   float NdotV = max(dot(N, V), 0.0);
   float NdotL = max(dot(N, L), 0.0);

   float ggx2 = GeometrySchlickGGX(NdotV, roughness);
   float ggx1 = GeometrySchlickGGX(NdotL, roughness);

   return ggx2 * ggx1;
}

void main() {
   vec3 N = normalize(Normal);
   vec3 V = normalize(viewPos - WorldPos);

   vec3 F0 = vec3(0.04);
   F0 = mix(F0, albedo, metallic);

   vec3 Lo = vec3(0.0f);
   for (int i = 0; i < 4; i++) {
       vec3 L = normalize(lightPosition[i] - WorldPos);
       vec3 H = normalize(V + L);
       float distance = length(lightPosition[i] - WorldPos);
       float attenuation = 1.0 / (distance * distance);
       vec3 radiance = lightColors[i] * attenuation;
       float NDF = DistributionGGX(N, H, roughness);
       float G = GeometrySmith(N, V, L, roughness);
       vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
       vec3 numerator = NDF * G * F;

       float NdotL = max(dot(N, L), 0.0);
       float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.0001;
       vec3 specular = numerator / denominator;
       vec3 Ks = F;
       vec3 Kd = vec3(1.0) - Ks;
       Lo += (Kd * albedo / PI + specular) * radiance * NdotL;
   }
   vec3 Ks = fresnelSchlick(max(dot(N, V), 0.0), F0);
   vec3 Kd = vec3(1.0) - Ks;
   Kd *= (1.0 - metallic);

   vec3 irradiance = texture(irradianceMap, N).rgb;
   vec3 diffuse = irradiance * albedo;
   vec3 ambient = Kd * diffuse * ao;
   vec3 color = ambient + Lo;

   color = color / (color + vec3(1.0f));
   color = pow(color, vec3(1.0 / 2.2));
   
   FragColor = vec4(color, 1.0);
}