#include "canvas.h"
#include "stb_image.h"

const string diffuseVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main(){\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
"}\n";

const string diffuseFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main(){\n"
"   FragColor = texture(texture_diffuse1, TexCoords);\n"
"}\n";

const string specularVertexShader =
"#version 330 core\n"
"void main() {\n"
"}\n";

const string specularFragShader =
"#version 330 core\n"
"void main() {\n"
"}\n";

const string phongVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string phongFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main(){\n"
"   vec4 kd = texture(texture_diffuse1, TexCoords);\n"
"   float ambientStrength = 0.1;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(Normal, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   float specularStrength = 0.2;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, Normal);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   FragColor = vec4(ambient + diffuse + specular, 1.0) * kd;\n"
"}\n";

const string blinnPhongVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string blinnPhongFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec2 TexCoords;\n"
"uniform vec3 ks;\n"
"uniform vec3 ka;\n"
"uniform vec3 intensity;\n"
"uniform vec3 ambLightIntensity;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main() {\n"
"   vec3 kd = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 viewDir = viewPos - FragPos;\n"
"   vec3 lightDir = lightPos - FragPos;\n"
"   vec3 l = normalize(lightDir);\n"
"   vec3 v = normalize(viewDir);\n"
"   vec3 half = normalize(v + l);\n"
"   vec3 realIntensity = intensity / dot(lightDir, lightDir);\n"
"   vec3 ambient = kd * ka * ambLightIntensity;\n"
"   vec3 diffuse = kd * realIntensity * max(dot(Normal, l), 0);\n"
"   vec3 specular = ks * realIntensity * pow(max(dot(Normal, half), 0), 150);\n"
"   vec3 result = ambient + diffuse + specular;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n";

const string bumpVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"layout(location = 3) in vec3 tangent;\n"
"layout(location = 4) in vec3 bitangent;\n"
"out vec2 TexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 TangentLightPos;\n"
"out vec3 TangentViewPos;\n"
"out vec3 TangentFragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"void main(){\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   mat3 normalMatrix = transpose(inverse(mat3(model)));\n"
"   Normal = normalMatrix * aNormal;\n"
"   vec3 T = normalize(normalMatrix * tangent);\n"
"   vec3 B = normalize(normalMatrix * bitangent);\n"
"   vec3 N = normalize(normalMatrix * aNormal);\n"
"   mat3 TBN = transpose(mat3(T, B, N));\n"
"   TangentLightPos = TBN * lightPos;\n"
"   TangentViewPos = TBN * viewPos;\n"
"   TangentFragPos = TBN * FragPos;\n"
"}\n";

const string bumpFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 FragPos;\n"
"in vec3 TangentLightPos;\n"
"in vec3 TangentViewPos;\n"
"in vec3 TangentFragPos;\n"
"in vec3 Normal;\n"
"uniform sampler2D texture_normal1;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform bool bump;\n"
"void main(){\n"
"   vec3 normal;\n"
"   if (bump) normal = texture(texture_normal1, TexCoords).rgb;\n"
"   else normal = normalize(Normal);\n"
"   normal = normalize(normal * 2.0 - 1.0);\n"
"   vec3 color = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 ambient = 0.1 * color;\n"
"   vec3 lightDir = normalize(TangentLightPos - TangentFragPos);\n"
"   float diff = max(dot(lightDir, normal), 0);\n"
"   vec3 diffuse = diff * color;\n"
"   vec3 viewDir = normalize(TangentViewPos - TangentFragPos);\n"
"   vec3 halfDir = normalize(lightDir + viewDir);\n"
"   float spec = pow(max(dot(normal, halfDir), 0), 64);\n"
"   vec3 specular = vec3(0.2) * spec;\n"
"   FragColor = vec4(ambient + diffuse + specular, 1.0f);\n"
"}\n";

const string mapVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"layout(location = 3) in vec3 tangent;\n"
"layout(location = 4) in vec3 bitangent;\n"
"out vec2 TexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 TangentLightPos;\n"
"out vec3 TangentViewPos;\n"
"out vec3 TangentFragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"void main(){\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   mat3 normalMatrix = transpose(inverse(mat3(model)));\n"
"   Normal = normalMatrix * aNormal;\n"
"   vec3 T = normalize(normalMatrix * tangent);\n"
"   vec3 B = normalize(normalMatrix * bitangent);\n"
"   vec3 N = normalize(normalMatrix * aNormal);\n"
"   mat3 TBN = transpose(mat3(T, B, N));\n"
"   TangentLightPos = TBN * lightPos;\n"
"   TangentViewPos = TBN * viewPos;\n"
"   TangentFragPos = TBN * FragPos;\n"
"}\n";

const string mapFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 FragPos;\n"
"in vec3 TangentLightPos;\n"
"in vec3 TangentViewPos;\n"
"in vec3 TangentFragPos;\n"
"in vec3 Normal;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform sampler2D texture_specular1;\n"
"uniform sampler2D texture_normal1;\n"
"uniform sampler2D texture_emission1;\n"
"uniform bool bump;\n"
"uniform bool sepmap;\n"
"void main(){\n"
"   vec3 normal;\n"
"   if (bump) normal = texture(texture_normal1, TexCoords).rgb;\n"
"   else normal = normalize(Normal);\n"
"   normal = normalize(normal * 2.0 - 1.0);\n"
"   vec3 color = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 ambient = 0.1 * color;\n"
"   vec3 lightDir = normalize(TangentLightPos - TangentFragPos);\n"
"   float diff = max(dot(lightDir, normal), 0);\n"
"   vec3 diffuse = diff * color;\n"
"   vec3 viewDir = normalize(TangentViewPos - TangentFragPos);\n"
"   vec3 halfDir = normalize(lightDir + viewDir);\n"
"   float spec = pow(max(dot(normal, halfDir), 0), 160);\n"
"   vec3 specular;\n"
"   if (sepmap) specular = spec * texture(texture_specular1, TexCoords).rgb;"
"   else specular = spec * vec3(0.2f); \n"
"   vec3 emission = texture(texture_emission1, TexCoords).rgb;\n"
"   FragColor = vec4(ambient + diffuse + specular + emission, 1.0f);\n"
"}\n";

const string cookTorranceVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"out vec3 WorldPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   TexCoords = aTexCoords;\n"
"   WorldPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(model) * aNormal;\n"
"   gl_Position = projection * view * vec4(WorldPos, 1.0);\n"
"}\n";

const string cookTorranceFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 WorldPos;\n"
"in vec3 Normal;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 albedo;\n"
"uniform float metallic;\n"
"uniform float roughness;\n"
"uniform float ao;\n"
"uniform vec3 lightPosition[4];\n"
"uniform vec3 lightColors[4];\n"
"const float PI = 3.14159265359;\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
"   return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);"
"}\n"
"float DistributionGGX(vec3 N, vec3 H, float roughness) {\n"
"   float a = roughness * roughness;\n"
"   float a2 = a * a;\n"
"   float NdotH = max(dot(N, H), 0.0);\n"
"   float NdotH2 = NdotH * NdotH;\n"
"   float nom = a2;\n"
"   float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
"   denom = PI * denom * denom;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySchlickGGX(float NdotV, float roughness) {\n"
"   float r = (roughness + 1.0);\n"
"   float k = (r * r) / 8.0;\n"
"   float nom = NdotV;\n"
"   float denom = NdotV * (1.0 - k) + k;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {\n"
"   float NdotV = max(dot(N, V), 0.0);\n"
"   float NdotL = max(dot(N, L), 0.0);\n"
"   float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
"   float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
"   return ggx1 * ggx2;\n"
"}\n"
"void main() {\n"
"   vec3 N = normalize(Normal);\n"
"   vec3 V = normalize(viewPos - WorldPos);\n"
"   vec3 F0 = vec3(0.04);\n"
"   F0 = mix(F0, albedo, metallic);\n"
"   vec3 Lo = vec3(0.0);\n"
"   for (int i = 0; i < 4; i++) {\n"
"       vec3 L = normalize(lightPosition[i] - WorldPos);\n"
"       vec3 H = normalize(L + V);\n"
"       float distance = length(lightPosition[i] - WorldPos);\n"
"       float attenuation = 1.0 / (distance * distance);\n"
"       vec3 radiance = lightColors[i] * attenuation;\n"
"       vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);\n"
"       float G = GeometrySmith(N, V, L, roughness);\n"
"       float NDF = DistributionGGX(N, H, roughness);\n"
"       vec3 numerator = NDF * G * F;\n"
"       float NdotL = max(dot(N, L), 0.0);\n"
"       float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.0001;\n"
"       vec3 specular = numerator / denominator;\n"
"       vec3 Ks = F;\n"
"       vec3 Kd = vec3(1.0) - Ks;\n"
"       Kd *= 1.0 - metallic;\n"
"       Lo += (Kd * albedo / PI + specular) * radiance * NdotL;\n"
"   }\n"
"   vec3 ambient = vec3(0.03) * albedo * ao;\n"
"   vec3 color = ambient + Lo;\n"
"   color = color / (color + vec3(1.0));\n"
"   color = pow(color, vec3(1.0 / 2.2));\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

const string pbrMapVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"out vec3 WorldPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   TexCoords = aTexCoords;\n"
"   WorldPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(model) * aNormal;\n"
"   gl_Position = projection * view * vec4(WorldPos, 1.0);\n"
"}\n";

const string pbrMapFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 WorldPos;\n"
"in vec3 Normal;\n"
"uniform vec3 viewPos;\n"
"uniform sampler2D albedoMap;\n"
"uniform sampler2D normalMap;\n"
"uniform sampler2D metallicMap;\n"
"uniform sampler2D roughnessMap;\n"
"uniform sampler2D aoMap;\n"
"uniform vec3 lightPosition[4];\n"
"uniform vec3 lightColors[4];\n"
"const float PI = 3.14159265359;\n"
"vec3 getNormalFromMap() {\n"
"   vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;\n"
"   vec3 Q1 = dFdx(WorldPos);\n"
"   vec3 Q2 = dFdy(WorldPos);\n"
"   vec2 st1 = dFdx(TexCoords);\n"
"   vec2 st2 = dFdy(TexCoords);\n"
"   vec3 N = normalize(Normal);\n"
"   vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);\n"
"   vec3 B = -normalize(cross(N, T));\n"
"   mat3 TBN = mat3(T, B, N);\n"
"   return normalize(TBN * tangentNormal);\n"
"}\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
"   return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);"
"}\n"
"float DistributionGGX(vec3 N, vec3 H, float roughness) {\n"
"   float a = roughness * roughness;\n"
"   float a2 = a * a;\n"
"   float NdotH = max(dot(N, H), 0.0);\n"
"   float NdotH2 = NdotH * NdotH;\n"
"   float nom = a2;\n"
"   float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
"   denom = PI * denom * denom;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySchlickGGX(float NdotV, float roughness) {\n"
"   float r = (roughness + 1.0);\n"
"   float k = (r * r) / 8.0;\n"
"   float nom = NdotV;\n"
"   float denom = NdotV * (1.0 - k) + k;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {\n"
"   float NdotV = max(dot(N, V), 0.0);\n"
"   float NdotL = max(dot(N, L), 0.0);\n"
"   float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
"   float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
"   return ggx1 * ggx2;\n"
"}\n"
"void main() {\n"
"   vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));\n"
"   float metallic = texture(metallicMap, TexCoords).r;\n"
"   float roughness = texture(roughnessMap, TexCoords).r;\n"
"   float ao = texture(aoMap, TexCoords).r;\n"
"   vec3 N = getNormalFromMap();\n"
"   vec3 V = normalize(viewPos - WorldPos);\n"
"   vec3 F0 = vec3(0.04);\n"
"   F0 = mix(F0, albedo, metallic);\n"
"   vec3 Lo = vec3(0.0);\n"
"   for (int i = 0; i < 4; i++) {\n"
"       vec3 L = normalize(lightPosition[i] - WorldPos);\n"
"       vec3 H = normalize(L + V);\n"
"       float distance = length(lightPosition[i] - WorldPos);\n"
"       float attenuation = 1.0 / (distance * distance);\n"
"       vec3 radiance = lightColors[i] * attenuation;\n"
"       vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);\n"
"       float G = GeometrySmith(N, V, L, roughness);\n"
"       float NDF = DistributionGGX(N, H, roughness);\n"
"       vec3 numerator = NDF * G * F;\n"
"       float NdotL = max(dot(N, L), 0.0);\n"
"       float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.0001;\n"
"       vec3 specular = numerator / denominator;\n"
"       vec3 Ks = F;\n"
"       vec3 Kd = vec3(1.0) - Ks;\n"
"       Kd *= 1.0 - metallic;\n"
"       Lo += (Kd * albedo / PI + specular) * radiance * NdotL;\n"
"   }\n"
"   vec3 ambient = vec3(0.03) * albedo * ao;\n"
"   vec3 color = ambient + Lo;\n"
"   color = color / (color + vec3(1.0));\n"
"   color = pow(color, vec3(1.0 / 2.2));\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

const string cubeMapVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec3 WorldPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"void main() {\n"
"   WorldPos = aPos;\n"
"   gl_Position = projection * view * vec4(WorldPos, 1.0);\n"
"}\n";

const string cubeMapFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"uniform sampler2D equirectangularMap;\n"
"const vec2 invAtan = vec2(0.1591, 0.3183);\n"
"vec2 sampleSphericalMap(vec3 v) {\n"
"   vec2 uv = vec2(atan(v.z, v.x), asin(v.y));\n"
"   uv *= invAtan;\n"
"   uv += 0.5;\n"
"   return uv;\n"
"}\n"
"void main() {\n"
"   vec2 uv = sampleSphericalMap(normalize(WorldPos));\n"
"   vec3 color = texture(equirectangularMap, uv).rgb;\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

const string backgroundVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"out vec3 WorldPos;\n"
"void main() {\n"
"   WorldPos = aPos;\n"
"   mat4 rotView = mat4(mat3(view));\n"
"   vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);\n"
"   gl_Position = clipPos.xyww;\n"
"}\n";

const string backgroundFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"uniform samplerCube environmentMap;\n"
"void main() {\n"
"   vec3 envColor = texture(environmentMap, WorldPos).rgb;\n"
"   envColor = envColor / (envColor + vec3(1.0f));\n"
"   envColor = pow(envColor, vec3(1.0/2.2));\n"
"   FragColor = vec4(envColor, 1.0);\n"
"}\n";

const string hdrboxVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"out vec3 WorldPos;\n"
"void main() {\n"
"   WorldPos = aPos;\n"
"   gl_Position = projection * view * vec4(WorldPos, 1.0);\n"
"}\n";

const string hdrboxFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"uniform sampler2D testMap;\n"
"const vec2 invAtan = vec2(0.1591, 0.3183);\n"
"vec2 sampleSphericalMap(vec3 v) {\n"
"   vec2 uv = vec2(atan(v.z, v.x), asin(v.y));\n"
"   uv *= invAtan;\n"
"   uv += 0.5;\n"
"   return uv;\n"
"}\n"
"void main() {\n"
"   vec2 uv = sampleSphericalMap(normalize(WorldPos));\n"
"   vec3 color = texture(testMap, uv).rgb;\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

const string irradianceConvolutionFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"uniform samplerCube environmentMap;\n"
"const float PI = 3.14159265359;\n"
"void main() {\n"
"   vec3 normal = normalize(WorldPos);\n"
"   vec3 irradiance = vec3(0.0f);\n"
"   vec3 up = vec3(0,1,0);\n"
"   vec3 right = normalize(cross(up, normal));\n"
"   up = normalize(cross(right, normal));\n"
"   float sampleDelta = 0.025;\n"
"   float nrSamples = 0.0;\n"
"   for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {\n"
"       for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {\n"
"           vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));\n"
"           vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;\n"
"           irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);\n"
"           nrSamples++;\n"
"       }\n"
"   }\n"
"   irradiance = PI * irradiance * (1.0 / nrSamples);\n"
"   FragColor = vec4(irradiance, 1.0);\n"
"}\n";

const string iblDiffusePbrFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoords;\n"
"uniform vec3 albedo;\n"
"uniform float roughness;\n"
"uniform float metallic;\n"
"uniform float ao;\n"
"uniform samplerCube irradianceMap;\n"
"uniform vec3 lightPosition[4];\n"
"uniform vec3 lightColors[4];\n"
"uniform vec3 viewPos;\n"
"const float PI = 3.15159265359;\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
"   return F0 +(1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
"}\n"
"float DistributionGGX(vec3 N, vec3 H, float roughness) {\n"
"   float alpha = roughness * roughness;\n"
"   float alpha2 = alpha * alpha;\n"
"   float NdotH = max(dot(N, H), 0.0);\n"
"   float NdotH2 = NdotH * NdotH;\n"
"   float nom = alpha2;\n"
"   float denom = (NdotH2 * (alpha2 - 1.0) + 1.0);\n"
"   denom = PI * denom * denom;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySchlickGGX(float NdotV, float roughness) {\n"
"   float r = roughness + 1.0;\n"
"   float k = (r * r) /8.0;\n"
"   float nom = NdotV;\n"
"   float denom = NdotV * (1.0 - k) + k;\n"
"   return nom / denom;\n"
"}\n"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {\n"
"   float NdotV = max(dot(N, V), 0.0);\n"
"   float NdotL = max(dot(N, L), 0.0);\n"
"   float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
"   float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
"   return ggx2 * ggx1;\n"
"}\n"
"void main() {\n"
"   vec3 N = normalize(Normal);\n"
"   vec3 V = normalize(viewPos - WorldPos);\n"
"   vec3 F0 = vec3(0.04);\n"
"   F0 = mix(F0, albedo, metallic);\n"
"   vec3 Lo = vec3(0.0f);\n"
"   for (int i = 0; i < 4; i++) {\n"
"       vec3 L = normalize(lightPosition[i] - WorldPos);\n"
"       vec3 H = normalize(V + L);\n"
"       float distance = length(lightPosition[i] - WorldPos);\n"
"       float attenuation = 1.0 / (distance * distance);\n"
"       vec3 radiance = lightColors[i] * attenuation;\n"
"       float NDF = DistributionGGX(N, H, roughness);\n"
"       float G = GeometrySmith(N, V, L, roughness);\n"
"       vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);\n"
"       vec3 numerator = NDF * G * F;\n"
"       float NdotL = max(dot(N, L), 0.0);\n"
"       float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.0001;\n"
"       vec3 specular = numerator / denominator;\n"
"       vec3 Ks = F;\n"
"       vec3 Kd = vec3(1.0) - Ks;\n"
"       Lo += (Kd * albedo / PI + specular) * radiance * NdotL;\n"
"   }\n"
"   vec3 Ks = fresnelSchlick(max(dot(N, V), 0.0), F0);\n"
"   vec3 Kd = vec3(1.0) - Ks;\n"
"   Kd *= (1.0 - metallic);\n"
"   vec3 irradiance = texture(irradianceMap, N).rgb;\n"
"   vec3 diffuse = irradiance * albedo;\n"
"   vec3 ambient = Kd * diffuse * ao;\n"
"   vec3 color = ambient + Lo;\n"
"   color = color / (color + vec3(1.0f));\n"
"   color = pow(color, vec3(1.0 / 2.2));\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f,  10.0f, 10.0f),
    glm::vec3(10.0f,  10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};

MyGLCanvas::MyGLCanvas(Widget* parent, Camera* _camera) : 
    nanogui::GLCanvas(parent), camera(_camera), untitleModel(1), untitleLight(1), preload(DIFFUSEIRRADIANCE), init(false) {
    using namespace nanogui;

    //modelList.emplace_back(new Model("..\\models\\sara\\sara.obj", "Model 1"));
    GLShader diffuseShder;
    diffuseShder.init("diffuse_shader", diffuseVertexShader, diffuseFragShader);
    diffuseShder.bind();
    shaderList.emplace_back(diffuseShder);

    GLShader phongShader;
    phongShader.init("phong_shader", phongVertexShader, phongFragShader);
    phongShader.bind();
    phongShader.setUniform("viewPos", camera->position);
    phongShader.setUniform("lightPos", vec3(2.5, 2.5, 2.5));
    phongShader.setUniform("lightColor", vec3(1.0, 1.0, 1.0));
    shaderList.emplace_back(phongShader);

    GLShader blinnPhongShder;
    blinnPhongShder.init("blinn_phong_shader", blinnPhongVertexShader, blinnPhongFragShader);
    blinnPhongShder.bind();
    blinnPhongShder.setUniform("lightPos", vec3(2.5));
    blinnPhongShder.setUniform("viewPos", camera->position);
    blinnPhongShder.setUniform("intensity", vec3(4.f));
    blinnPhongShder.setUniform("ka", vec3(0.01f));
    blinnPhongShder.setUniform("ambLightIntensity", vec3(20.f));
    blinnPhongShder.setUniform("ks", vec3(0.1f));
    shaderList.emplace_back(blinnPhongShder);

    GLShader bumpShader;
    bumpShader.init("bump_shader", bumpVertexShader, bumpFragShader);
    bumpShader.bind();
    bumpShader.setUniform("lightPos", vec3(2.5));
    bumpShader.setUniform("viewPos", camera->position);
    shaderList.emplace_back(bumpShader);

    //GLShader mapShader;
    //mapShader.init("map_shader", mapVertexShader, mapFragShader);
    //mapShader.bind();
    //mapShader.setUniform("lightPos", vec3(2.5));
    //mapShader.setUniform("viewPos", camera->position);
    //shaderList.emplace_back(mapShader);

    GLShader cooktorranceShader;
    cooktorranceShader.init("cook_torrance_pbr_shader", cookTorranceVertexShader, cookTorranceFragShader);
    cooktorranceShader.bind();
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        cooktorranceShader.setUniform("lightPosition[" + std::to_string(i) + "]", lightPositions[i]);
        cooktorranceShader.setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    cooktorranceShader.setUniform("roughness", 0.2);
    cooktorranceShader.setUniform("metallic", 0.8);
    cooktorranceShader.setUniform("ao", 1.0f);
    cooktorranceShader.setUniform("albedo", vec3(0.5f, 0.0f, 0.0f));
    shaderList.emplace_back(cooktorranceShader);

    GLShader pbrMapShader;
    pbrMapShader.init("pbr_map_shader", pbrMapVertexShader, pbrMapFragShader);
    shaderList.emplace_back(pbrMapShader);

    GLShader cubeMapShader;
    cubeMapShader.init("ibl_cube_hdr_map_shader", cubeMapVertexShader, cubeMapFragShader);
    shaderList.emplace_back(cubeMapShader);

    GLShader backgroundShader;
    backgroundShader.init("simple_skybox_shader", backgroundVertexShader, backgroundFragShader);
    shaderList.emplace_back(backgroundShader);

    GLShader hdrboxShader;
    hdrboxShader.init("test_shader", hdrboxVertexShader, hdrboxFragShader);
    shaderList.emplace_back(hdrboxShader);

    GLShader irradianceConvolutionShader;
    irradianceConvolutionShader.init("irradiance_convolution_shader", cubeMapVertexShader, irradianceConvolutionFragShader);
    shaderList.emplace_back(irradianceConvolutionShader);

    GLShader diffusePbrShader;
    diffusePbrShader.init("diffuse_ibl_shader", cookTorranceVertexShader, iblDiffusePbrFragShader);
    diffusePbrShader.bind();
    diffusePbrShader.setUniform("ao", 1.0f);
    diffusePbrShader.setUniform("albedo", vec3(0.5f, 0.0f, 0.0f));
    shaderList.emplace_back(diffusePbrShader);

    //lightShader.init(
    //    "light_shader",

    //    "#version 330 core\n"
    //    "in vec3 position;\n"
    //    "uniform mat4 lightMVP;\n"
    //    "void main() {\n"
    //    "   gl_Position = lightMVP * vec4(position, 1.0);\n"
    //    "}\n",

    //    "#version 330 core\n"
    //    "out vec4 FragColor;\n"

    //    "void main() {\n"
    //    "   FragColor = vec4(1.0);\n"
    //    "}\n"
    //);

    //MatrixXu indices(3, 12); /* Draw a cube */
    //indices.col(0) << 0, 1, 3;
    //indices.col(1) << 3, 2, 1;
    //indices.col(2) << 3, 2, 6;
    //indices.col(3) << 6, 7, 3;
    //indices.col(4) << 7, 6, 5;
    //indices.col(5) << 5, 4, 7;
    //indices.col(6) << 4, 5, 1;
    //indices.col(7) << 1, 0, 4;
    //indices.col(8) << 4, 0, 3;
    //indices.col(9) << 3, 7, 4;
    //indices.col(10) << 5, 6, 2;
    //indices.col(11) << 2, 1, 5;

    //MatrixXf positions(3, 8);
    //positions.col(0) << 2, 3, 3;
    //positions.col(1) << 2, 3, 2;
    //positions.col(2) << 3, 3, 2;
    //positions.col(3) << 3, 3, 3;
    //positions.col(4) << 2, 2, 3;
    //positions.col(5) << 2, 2, 2;
    //positions.col(6) << 3, 2, 2;
    //positions.col(7) << 3, 2, 3;

    //MatrixXf colors(3, 12);
    //colors.col(0) << 1, 0, 0;
    //colors.col(1) << 0, 1, 0;
    //colors.col(2) << 1, 1, 0;
    //colors.col(3) << 0, 0, 1;
    //colors.col(4) << 1, 0, 1;
    //colors.col(5) << 0, 1, 1;
    //colors.col(6) << 1, 1, 1;
    //colors.col(7) << 0.5, 0.5, 0.5;
    //colors.col(8) << 1, 0, 0.5;
    //colors.col(9) << 1, 0.5, 0;
    //colors.col(10) << 0.5, 1, 0;
    //colors.col(11) << 0.5, 1, 0.5;
    

    //camera->cameraPos = vec3(0.0f, 0.0f, 3.0f);
    //camera->cameraFront = vec3(0.0f, 0.0f, -1.0f);
    //camera->cameraUp = vec3(0.0f, 1.0f, 0.0f);
    //camera->cameraTarget = vec3(0, 0, 0);
    //camera->translateSpeed = 0.01;
    //camera->scaleSpeed = 0.15;
    //camera->sensitivity = 0.2;

    //translate = vec3(0.f);
    //scale = vec3(1.f);
    //model = mat4(1.0f);
    //model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    //mShader.bind();
 /*   mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);*/

    //lightShader.bind();
    //lightShader.uploadIndices(indices);
    //lightShader.uploadAttrib("position", positions);
}

void MyGLCanvas::drawGL() {
    using namespace nanogui;
    if (preload == PRELOAD_NONE) {
        if (modelList.size() == 0) return;
        updateCamera();
        //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
        //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
        //std::cout << std::endl;
        view = lookAt(camera->position, camera->target, camera->up);
        projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
        //view = lookAt(vec3(0.0f, 0.0f, 3.f), vec3(0.f, 0.f, 2.f), vec3(0, 1.f, 0));
        //projection = glm::perspective(radians(camera->fov), 1.0f, 0.1f, 100.0f);
        //projection = glm::perspective(radians(45.f), 1.0f, 0.1f, 100.0f);

        //model = glm::translate(model, translate);
        //model = glm::scale(model, scale);
        //mvp = projection * view * model;

        //mShader.setUniform("model", model);
        //mShader.setUniform("view", view);
        //mShader.setUniform("projection", projection);
        for (int i = 0; i < modelList.size(); i++) {
            model[0][0] = 1.0f;
            model[0][1] = 0.0f;
            model[0][2] = 0.0f;
            model[0][3] = 0.0f;
            model[1][0] = 0.0f;
            model[1][1] = 1.0f;
            model[1][2] = 0.0f;
            model[1][3] = 0.0f;
            model[2][0] = 0.0f;
            model[2][1] = 0.0f;
            model[2][2] = 1.0f;
            model[2][3] = 0.0f;
            model[3][0] = 0.0f;
            model[3][1] = 0.0f;
            model[3][2] = 0.0f;
            model[3][3] = 1.0f;
            model = glm::translate(model, modelList[i]->translate);
            model = glm::scale(model, modelList[i]->scale);
            shaderList[modelList[i]->shaderIndex].bind();
            shaderList[modelList[i]->shaderIndex].setUniform("model", model);
            shaderList[modelList[i]->shaderIndex].setUniform("view", view);
            shaderList[modelList[i]->shaderIndex].setUniform("projection", projection);
            shaderList[modelList[i]->shaderIndex].setUniform("viewPos", camera->position);
            shaderList[modelList[i]->shaderIndex].setUniform("roughness", modelList[i]->roughenss);
            shaderList[modelList[i]->shaderIndex].setUniform("metallic", modelList[i]->metallic);
            glEnable(GL_DEPTH_TEST);
            //modelList[i]->draw(mShader);
            modelList[i]->draw(shaderList[modelList[i]->shaderIndex]);
            glDisable(GL_DEPTH_TEST);
        }
        //lightShader.bind();
        //lightShader.setUniform("lightMVP", projection * view);
        //glEnable(GL_DEPTH_TEST);
        /* Draw 12 triangles starting at index 0 */
        //mShader.drawIndexed(GL_TRIANGLES, 0, 12);
        //lightShader.drawIndexed(GL_TRIANGLES, 0, 12);
        //glDisable(GL_DEPTH_TEST);
    }
    else if (preload == PBRMAPBALLS) {
        glEnable(GL_DEPTH_TEST);
        preloadPbrMapBalls();
    }
    else if (preload == DIFFUSEIRRADIANCE) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        preloadDiffuseIrradiance();
    }
    else if (preload == KLUDIAVALENTZ) {
        preloadKlaudiaValentz();
    }
}

string MyGLCanvas::addModel(const string& path, const string& name) {
    string id;
    if (name == "" || name == "untitle") {
        id = "untitle." + untitleModel;
        modelList.emplace_back(new Model(path, id));
        untitleModel++;
    }
    else {
        int num = 0;
        string realname = name.substr(0, name.find_last_of('.'));
        for (int i = 0; i < modelList.size(); i++) {
            if (modelList[i]->name.substr(0, modelList[i]->name.find_last_of('.')) == realname) {
                num++;
            }
        }
        if (num > 0) {
            id = realname + "." + std::to_string(num);
            modelList.emplace_back(new Model(path, id));
        }
        else {
            id = name;
            modelList.emplace_back(new Model(path, name));
        }
    }
    return id;
}

string MyGLCanvas::addLight(const string& name, int type, vec3 _position, vec3 _color, vec3 _intensity) {
    //if (type < 0 || type > lightShaderList.size()) type = 0;
    string id;
    if (name == "" || name == "untitle") {
        id = "untitle." + untitleLight;
        lightList.emplace_back(new Light(id, type, _position, _color, _intensity));
        untitleLight++;
    }
    else {
        int num = 0;
        string realname = name.substr(0, name.find_last_of('.'));
        for (int i = 0; i < lightList.size(); i++) {
            if (lightList[i]->name.substr(0, lightList[i]->name.find_last_of('.')) == realname) {
                num++;
            }
        }
        if (num > 0) {
            id = realname + "." + std::to_string(num);
            lightList.emplace_back(new Light(id, type, _position, _color, _intensity));
        }
        else {
            id = name;
            lightList.emplace_back(new Light(name, type, _position, _color, _intensity));
        }
    }
    return id;
}

Light* MyGLCanvas::findLight(const string& id) {
    for (int i = 0; i < lightList.size(); i++) {
        if (lightList[i]->name == id) return lightList[i];
    }
    return nullptr;
}

Model* MyGLCanvas::findModel(const string& id) {
    for (int i = 0; i < modelList.size(); i++) {
        if (modelList[i]->name == id) return modelList[i];
    }
    return nullptr;
}

Light* MyGLCanvas::firstLight() {
    return lightList[0];
}

Model* MyGLCanvas::firstModel() {
    return modelList[0];
}

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

void MyGLCanvas::preloadPbrMapBalls() {
    shaderList[5].bind();
    if (!init) {
        shaderList[5].setUniform("albedoMap", 1);
        shaderList[5].setUniform("normalMap", 2);
        shaderList[5].setUniform("metallicMap", 3);
        shaderList[5].setUniform("roughnessMap", 4);
        shaderList[5].setUniform("aoMap", 5);
        unsigned int albedo = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_basecolor.png");
        unsigned int normal = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_normal.png");
        unsigned int metallic = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_metallic.png");
        unsigned int roughness = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_roughness.png");
        unsigned int ao = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_ambient.png");
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, albedo);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metallic);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, roughness);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, ao);
        init = true;
    }
    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
    shaderList[5].setUniform("view", view);
    shaderList[5].setUniform("projection", projection);
    shaderList[5].setUniform("viewPos", camera->position);

    for (unsigned int i = 0; i < 4; ++i)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shaderList[5].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
        shaderList[5].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row)
    {
        for (int col = 0; col < nrColumns; ++col)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns / 2)) * spacing,
                (float)(row - (nrRows / 2)) * spacing,
                0.0f
            ));
            shaderList[5].setUniform("model", model);
            renderSphere();
        }
    }
}

unsigned int envCubeMap;
unsigned int irradianceMap;
void MyGLCanvas::preloadDiffuseIrradiance() {
    if (!init) {
        shaderList[7].bind();
        shaderList[7].setUniform("environmentMap", 0);

        // pbr: setup framebuffer
        // ----------------------
        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        // pbr: load the HDR environment map
        // ---------------------------------
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        float* data = stbi_loadf("..\\models\\HDR\\Newport_Loft\\Newport_Loft_Ref.hdr", &width, &height, &nrComponents, 0);
        unsigned int hdrTexture;
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load HDR image." << std::endl;
        }

        // pbr: setup cubemap to render to and attach to framebuffer
        // ---------------------------------------------------------
        glGenTextures(1, &envCubeMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // pbr: convert HDR equirectangular environment map to cubemap equivalent
        // ----------------------------------------------------------------------
        shaderList[6].bind();
        shaderList[6].setUniform("equirectangularMap", 0);
        shaderList[6].setUniform("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        
        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glScissor(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            shaderList[6].setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // convolute irradiance
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
        shaderList[9].bind();
        shaderList[9].setUniform("environmentMap", 0);
        shaderList[9].setUniform("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        glViewport(0, 0, 32, 32);
        glScissor(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; i++) {
            shaderList[9].setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        GLint storedViewport[4];
        glGetIntegerv(GL_VIEWPORT, storedViewport);
        glViewport(storedViewport[0], storedViewport[1], storedViewport[2], storedViewport[3]);
        glScissor(storedViewport[0], storedViewport[1], storedViewport[2], storedViewport[3]);
        init = true;
    }
    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);

    //shaderList[8].bind();
    //shaderList[8].setUniform("testMap", 0);
    //shaderList[8].setUniform("projection", projection);
    //shaderList[8].setUniform("view", view);
    //stbi_set_flip_vertically_on_load(true);
    //int width, height, nrComponents;
    //float* data = stbi_loadf("..\\models\\HDR\\Newport_Loft\\Newport_Loft_Ref.hdr", &width, &height, &nrComponents, 0);
    //unsigned int hdrTexture;
    //if (data)
    //{
    //    glGenTextures(1, &hdrTexture);
    //    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //    stbi_image_free(data);
    //}
    //else
    //{
    //    std::cout << "Failed to load HDR image." << std::endl;
    //}
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //renderCube();

    //shaderList[4].bind();
    //shaderList[4].setUniform("view", view);
    //shaderList[4].setUniform("projection", projection);
    //shaderList[4].setUniform("viewPos", camera->position);
    //for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    //{
    //    glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
    //    shaderList[4].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
    //    shaderList[4].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    //}
    //// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
    //glm::mat4 model = glm::mat4(1.0f);
    //for (int row = 0; row < nrRows; ++row)
    //{
    //    shaderList[4].setUniform("metallic", (float)row / (float)nrRows);
    //    for (int col = 0; col < nrColumns; ++col)
    //    {
    //        // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
    //        // on direct lighting.
    //        shaderList[4].setUniform("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
    //        model = glm::mat4(1.0f);
    //        model = glm::translate(model, glm::vec3(
    //            (float)(col - (nrColumns / 2)) * spacing,
    //            (float)(row - (nrRows / 2)) * spacing,
    //            -2.0f
    //        ));
    //        shaderList[4].setUniform("model", model);
    //        renderSphere();
    //    }
    //}

    shaderList[10].bind();
    shaderList[10].setUniform("view", view);
    shaderList[10].setUniform("projection", projection);
    shaderList[10].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shaderList[10].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
        shaderList[10].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row)
    {
        shaderList[10].setUniform("metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col)
        {
            // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
            // on direct lighting.
            shaderList[10].setUniform("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns / 2)) * spacing,
                (float)(row - (nrRows / 2)) * spacing,
                -2.0f
            ));
            shaderList[10].setUniform("model", model);
            renderSphere();
        }
    }

    // render skybox (render as last to prevent overdraw)
    shaderList[7].bind();
    shaderList[7].setUniform("view", view);
    shaderList[7].setUniform("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    renderCube();
}

unsigned int klaudiaVAO = 0;
void MyGLCanvas::preloadKlaudiaValentz() {
    string id = addModel("..\\models\\KlaudiaValentz\\KlaudiaValentz.obj", "Klaudia");
    Model* model = modelList[0];
    model->shaderIndex = 4;
    model->translate = vec3(0.0);
    loadTexture("..\\models\\KlaudiaValentz\\RaycastEffects\\Boyd\\");
}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void MyGLCanvas::renderSphere() {
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void MyGLCanvas::renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int MyGLCanvas::loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

bool MyGLCanvas::scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) {
    // p是鼠标位置，rel是鼠标的滚动，(0,1)是前滚，(0,-1)是后滚
    //std::cout << "scroll" << std::endl;
    //std::cout << p.x() << " " << p.y() << std::endl;
    //std::cout << rel.x() << " " << rel.y() << std::endl;
    //if (rel.y() == 1) {
    //    camera->cameraPos += camera->scaleSpeed * camera->cameraFront;
    //    return true;
    //}
    //else if (rel.y() == -1) {
    //    camera->cameraPos -= camera->scaleSpeed * camera->cameraFront;
    //    return true;
    //}
    //if (rel.y() == 1) {
    //    camera->fov -= 2;
    //    if (camera->fov < 1.0f) {
    //        camera->fov = 1.0f;
    //    }
    //    else if (camera->fov > 60.0f) {
    //        camera->fov = 60.0f;
    //    }
    //    return true;
    //}
    //else if (rel.y() == -1) {
    //    camera->fov += 2;
    //    if (camera->fov < 1.0f) {
    //        camera->fov = 1.0f;
    //    }
    //    else if (camera->fov > 45.0f) {
    //        camera->fov = 45.0f;
    //    }
    //    return true;
    //}
    if (rel.y() == 1) {
        camera->dollyDelta += 2.f;
        return true;
    }
    else if (rel.y() == -1) {
        camera->dollyDelta -= 2.f;
        return true;
    }
    return false;
}

//bool MyGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
//    // 左键是1，右键是2，p是鼠标位置，rel是相对于上一步朝哪个方向移动
//    //std::cout << button << std::endl;
//    //std::cout << p.x() << " " << p.y() << std::endl;
//    //std::cout << rel.x() << " " << rel.y() << std::endl;
//    if (button == 1) {
//        //if (camera->firstMouse) {
//        //    camera->lastX = p.x();
//        //    camera->lastY = p.y();
//        //    camera->firstMouse = false;
//        //}
//
//        //float xOffset = p.x() - camera->lastX;
//        //float yOffset = p.y() - camera->lastY;
//        //std::cout << p.x() << " " << p.y() << std::endl;
//        //std::cout << camera->lastX << " " << camera->lastY << std::endl;
//        //std::cout << std::endl;
//        //camera->lastX = p.x();
//        //camera->lastY = p.y(); 
//
//        //xOffset *= camera->sensitivity;
//        //yOffset *= camera->sensitivity;
//
//        //camera->yaw += xOffset;
//        //camera->pitch -= yOffset;
//        //if (camera->pitch > 89.0f) {
//        //    camera->pitch = 89.0f;
//        //}
//        //if (camera->pitch < -89.0f) {
//        //    camera->pitch = -89.0f;
//        //}
//
//        //vec3 front;
//        //front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
//        //front.y = sin(radians(camera->pitch));
//        //front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
//        //camera->cameraFront = front;
//        //return true;
//
//        vec3 from_target = camera->cameraPos - camera->cameraTarget;
//        float radius = 3.0f;
//        float theta = (float)atan2(from_target.x, from_target.z);  /* azimuth */
//        float phi = (float)acos(from_target.y / radius);           /* polar */
//        vec3 offset;
//
//        offset.x = radius * (float)sin(phi) * (float)sin(theta);
//        offset.y = radius * (float)cos(phi);
//        offset.z = radius * (float)sin(phi) * (float)cos(theta);
//        camera->cameraPos += offset;
//        return true;
//    }
//    else if (button == 2) {
//        if (rel.x() == 1) {
//            std::cout << "1" << std::endl;
//            camera->cameraPos -= normalize(cross(camera->cameraFront, camera->cameraUp)) * camera->translateSpeed;
//        }
//        else if (rel.x() == -1) {
//            std::cout << "2" << std::endl;
//            camera->cameraPos += normalize(cross(camera->cameraFront, camera->cameraUp)) * camera->translateSpeed;
//        }
//        else if (rel.y() == 1) {
//
//        } 
//    }
//    return false;
//}

// 可以用来实现fps镜头
//bool MyGLCanvas::mouseMotionEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
//    if (camera->firstMouse) {
//        camera->lastX = p.x();
//        camera->lastY = p.y();
//        camera->firstMouse = false;
//    }
//
//    float xOffset = p.x() - camera->lastX;
//    float yOffset = p.y() - camera->lastY;
//    std::cout << p.x() << " " << p.y() << std::endl;
//    std::cout << camera->lastX << " " << camera->lastY << std::endl;
//    std::cout << std::endl;
//    camera->lastX = p.x();
//    camera->lastY = p.y();
//
//    xOffset *= camera->sensitivity;
//    yOffset *= camera->sensitivity;
//
//    camera->yaw += xOffset;
//    camera->pitch += yOffset;
//    if (camera->pitch > 89.0f) {
//        camera->pitch = 89.0f;
//    }
//    if (camera->pitch < -89.0f) {
//        camera->pitch = -89.0f;
//    }
//
//    vec3 front;
//    front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
//    front.y = sin(radians(camera->pitch));
//    front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
//    camera->cameraFront = front;
//    return true;
//}

bool MyGLCanvas::mouseButtonEvent(const nanogui::Vector2i& p, int button, bool down, int modifiers) {
    // button左键是0，右键是1
    // down 按下是1，松开是0
    if (!focused()) {
        requestFocus();
    }
    vec2 cursorPos = vec2(p.x(), p.y());
    camera->cursor = cursorPos;
    if (button == GLFW_MOUSE_BUTTON_1) {
        if (down) {
            camera->isOrbiting = true;
            camera->orbitPos = cursorPos;
        }
        else {
            vec2 posDelta = getPosDelta(camera->orbitPos, cursorPos);
            camera->isOrbiting = false;
            camera->orbitDelta += posDelta;
        }
        //std::cout << camera->orbitPos.x << " " << camera->orbitPos.y << std::endl;
        return true;
    }
    else if (button == GLFW_MOUSE_BUTTON_2) {
        if (down) {
            camera->isPanning = true;
            camera->panPos = cursorPos;
        }
        else {
            vec2 posDelta = getPosDelta(camera->panPos, cursorPos);
            camera->isPanning = false;
            camera->panDelta += posDelta;
        }
        return true;
    }
    return false;
}

bool MyGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
    //vec2 cursorPos = vec2(p.x(), p.y());
    //camera->cursor = cursorPos;
    //if (button == 1) {
    //    //camera->orbitPos = cursorPos;
    //    //std::cout << camera->orbitPos.x << " " << camera->orbitPos.y << std::endl;
    //    return true;
    //}
    //else if (button == 2) {
    //    //camera->panPos = cursorPos;
    //    return true;
    //}
    camera->cursor.x = p.x();
    camera->cursor.y = p.y();
    return true;
}

bool MyGLCanvas::mouseMotionEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
    camera->cursor = vec2(p.x(), p.y());
    return true;
}

vec2 MyGLCanvas::getPosDelta(vec2 old_pos, vec2 new_pos) {
    return (new_pos - old_pos) / 400.0f;
}

void MyGLCanvas::updateCamera() {
    vec2 cursorPos = camera->cursor;
    //std::cout << cursorPos.x << " " << cursorPos.y << std::endl;
    if (camera->isOrbiting) {
        vec2 posDelta = getPosDelta(camera->orbitPos, cursorPos);
        camera->orbitDelta += posDelta;
        camera->orbitPos = cursorPos;
    }
    if (camera->isPanning) {
        vec2 posDelta = getPosDelta(camera->panPos, cursorPos);
        /*std::cout << posDelta.x << " " << posDelta.y << std::endl;*/
        camera->panDelta += posDelta;
        //std::cout << camera->panDelta.x << " " << camera->panDelta.y << std::endl;
        camera->panPos = cursorPos;
    }
    camera->orbit = camera->orbitDelta;
    camera->pan = camera->panDelta;
    //std::cout << camera->pan.x << " " << camera->pan.y << std::endl;
    camera->dolly = camera->dollyDelta;
    //std::cout << camera->dollyDelta << std::endl;
    //if (count1 == 1) {
    //    std::cout << camera->pan.x << " " << camera->pan.y  << std::endl;
    //    std::cout << camera->orbit.x << " " << camera->orbit.y << std::endl;
    //    std::cout << std::endl;
    //    count1++;
    //}
    //if (count1 == 1) {
    //    std::cout << camera->position.x << " ei " << camera->position.y << " " << camera->position.z << std::endl;
    //    std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //    std::cout << std::endl;
    //    count1++;
    //}
    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
    camera->updateTransform();
    
    camera->dollyDelta = 0;
    camera->orbitDelta.x = 0;
    camera->orbitDelta.y = 0;
    camera->panDelta.x = 0;
    camera->panDelta.y = 0;

    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
}

bool MyGLCanvas::keyboardEvent(int key, int scancode, int action, int modifiers) {
    //std::cout << "debgu2" << std::endl;
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //    setVisible(false);
    //    return false;
    //}
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera->recoverDefaultLocation();
    }
    return true;
}