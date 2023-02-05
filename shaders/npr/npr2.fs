#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 lightFragPos;

uniform sampler2D ilmTexture1;
uniform sampler2D texture_diffuse1;
uniform sampler2D sdfMap;
uniform sampler2D hairLightTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 _AmbientColor;
uniform vec3 _LightColor;
uniform vec3 _SpecularColor;
uniform float _SpecularRange;
uniform vec3 _RimColor;
uniform float _RimAmount;
uniform float _MinRim;
uniform float _MaxRim;
uniform float _Bloom;
uniform float _RimBloomExp;
uniform float _ShadowAttWeight;
uniform float _Atten;
uniform float _DividLineH;
uniform float _DividLineM;
uniform float _DividLineD;
uniform float _BoundSharp;

float sigmoid(float x, float center, float sharp) {
   return 1 / (1 + pow(10000, -3 * sharp * (x - center)));
}

void main() {
   vec3 color = texture(texture_diffuse1, TexCoords).rgb;
   vec3 ilmTex = texture(ilmTexture1, TexCoords).rgb;
   vec3 normal = normalize(Normal);
   if(!gl_FrontFacing) normal = -normal;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 lightDir = normalize(lightPos - FragPos);
   float NdotL = max(dot(normal, lightDir), 0);
   float wrapLambert = NdotL + ilmTex.g;
   float shadowStep = smoothstep(0.9, 0.95, wrapLambert);
   vec3 shadow = mix(_AmbientColor, _LightColor, shadowStep);
   float NoL = dot(normal, lightDir) + _ShadowAttWeight * (_Atten - 1);
   float HLightSig = sigmoid(NoL, _DividLineH, _BoundSharp);
   float MidSig = sigmoid(NoL, _DividLineM, _BoundSharp);
   float DarkSig = sigmoid(NoL, _DividLineD, _BoundSharp);
   float HLightWin = HLightSig;
   float MidLWin = MidSig - HLightSig;
   float MidDWin = DarkSig - MidSig;
   float DarkWin = 1 - DarkSig;
   float intensity = HLightWin * 1.0 + MidLWin * 0.8 + MidDWin * 0.5 + DarkWin * 0.3;
   vec3 diffuse = color * mix(_AmbientColor, _LightColor, intensity);
   vec3 halfwayDir = normalize(lightDir + viewDir);
   float spec = pow(max(dot(normal, halfwayDir), 0), 128);
   float specularRange = step(_SpecularRange, (spec + clamp(dot(normal.xz, lightDir.xz), 0.0, 1.0)) * ilmTex.r * ilmTex.b);
   vec3 specular = specularRange * _SpecularColor;
   vec3 Front = vec3(0, 0, -1.0);
   float f = 1.0 - clamp(dot(viewDir, normal), 0.0, 1.0);
   float rim = smoothstep(_MinRim, _MaxRim, f);
   rim = smoothstep(0, _RimAmount, rim);
// 使用mask控制bloom区域
   vec3 rimColor = texture(hairLightTexture, TexCoords).g * rim * _RimColor.rgb;
   float rimBloom;
// 背光直接是1， 不背光
   if (dot(normal, lightDir) < 0.2 || f < 0.6) {
       rimBloom = 1.0;
   } else {
       rimBloom = pow(f, _RimBloomExp) * _Bloom * dot(normal, lightDir);
   }
//"   vec3 lighting = rimColor + (shadow + specular) * color;
   vec3 lighting = diffuse + (rimColor + specular) * color;
   FragColor = vec4(lighting, rimBloom);
//"   FragColor = vec4(1.0);
//"   FragColor = vec4(mix(_AmbientColor, _LightColor, shadowStep), 1.0);
//"   FragColor = vec4(specular, 1.0);
}