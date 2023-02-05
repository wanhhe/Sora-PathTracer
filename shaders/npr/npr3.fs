#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D ilmTexture1;
uniform sampler2D texture_diffuse1;
uniform sampler2D sdfMap;
uniform sampler2D hairLightTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 _AmbientColor;
uniform vec3 _LightColor;
uniform float _SpecularRange;
uniform vec3 _RimColor;
uniform float _RimAmount;
uniform float _MinRim;
uniform float _MaxRim;
uniform float _Bloom;
uniform float _RimBloomExp;
uniform float _ShadowAttWeight;
uniform float _Atten;
uniform float _DividSharpness;
uniform float _DividLineSpec;
uniform float _Glossiness;
uniform float _AOWeight;
uniform float _DividLineM;
uniform float _DividLineD;
uniform vec3 _DarkFaceColor;
uniform vec3 _DeepDarkColor;
uniform vec3 _DiffuseBright;
uniform vec3 _FresnelColor;
uniform float _FresnelEff;

float sigmoid(float x, float center, float sharp) {
   return 1 / (1 + pow(10000, -3 * sharp * (x - center)));
}

float D_GGX(float a2, float NoH) {
   float d = (NoH * a2 - NoH) * NoH + 1;
   return a2 / (3.1415926 * d * d);
}

float normal2Ndc(float x) {
   return x * 2.0 - 1.0;
}

float ndc2Normal(float x) {
   return x * 0.5 + 0.5;
}

vec3 warp(vec3 x, vec3 w) {
   return (x + w) / (vec3(1.0) + w);
}

vec3 FresnelSchlick(float VoN, vec3 F0) {
   return F0 + (1 - F0) * pow(1 - VoN, 5);
}

vec3 FresnelExtend(float VoN, vec3 F0) {
   return F0 + (1- F0) * pow(1 - VoN, 3);
}

void main() {
   vec3 color = texture(texture_diffuse1, TexCoords).rgb;
   vec3 ilmTex = texture(ilmTexture1, TexCoords).rgb;
   vec3 normal = normalize(Normal);
   if(!gl_FrontFacing) normal = -normal;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 lightDir = normalize(lightPos - FragPos);
   vec3 halfwayDir = normalize(viewDir + lightDir);
   float NdotL = max(dot(normal, lightDir), 0);
   float NoL = dot(normal, lightDir);
   float NdotH = max(dot(normal, halfwayDir), 0);
   float NoH = dot(normal, halfwayDir);
   float NoV = dot(normal, viewDir);
   float VoL = dot(viewDir, lightDir);
   float wrapLambert = NdotL + ilmTex.g;
   float shadowStep = smoothstep(0.9, 0.95, wrapLambert);
   vec3 shadow = mix(_AmbientColor, _LightColor, shadowStep);
   float roughness = 0.95 - 0.95 * ilmTex.r * _Glossiness;
   float boundSharp = 9.5 * pow(roughness - 1, 2) + 0.5;
   vec3 fresnel = FresnelExtend(NoV, vec3(0.1));
   vec3 fresnelResult = _FresnelEff * fresnel * (1 - VoL) / 2;
   float NDF0 = D_GGX(roughness * roughness, 1.0);
   float NDF_HBound = NDF0 * _DividLineSpec;
   float NDF = D_GGX(roughness * roughness, NoH) + _ShadowAttWeight * (_Atten - 1);
   float specularWin = sigmoid(NDF, NDF_HBound, boundSharp * _DividSharpness);
   float specWeight = specularWin * (NDF0 + NDF_HBound) / 2.0 * ilmTex.b;
   vec3 specular = specWeight * _LightColor;
   float lambert = NoL + _AOWeight * normal2Ndc(ilmTex.g)+ _ShadowAttWeight * (_Atten - 1);
   float MidSig = sigmoid(lambert, _DividLineM, boundSharp * _DividSharpness);
   float DarkSig = sigmoid(lambert, _DividLineD, boundSharp * _DividSharpness);
   float MidLWin = MidSig;
   float MidDWin = DarkSig - MidSig;
   float DarkWin = 1 - DarkSig;
   vec3 diffuseWeight = vec3(MidLWin * (1 + ndc2Normal(_DividLineM)) / 2.0);
   diffuseWeight += vec3(MidDWin * (ndc2Normal(_DividLineM) + ndc2Normal(_DividLineD)) / 2.0) * _DarkFaceColor * 3 / (_DarkFaceColor.r + _DarkFaceColor.g + _DarkFaceColor.b);
   diffuseWeight += vec3(DarkWin * ndc2Normal(_DividLineD)) * _DeepDarkColor * 3 / (_DeepDarkColor.r + _DeepDarkColor.g + _DeepDarkColor.b);
   diffuseWeight = warp(diffuseWeight, _DiffuseBright.xxx);
   vec3 diffuse = diffuseWeight * color;
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
   vec3 lighting = specular + (1.0 - specWeight) * diffuse + rimColor * color + fresnelResult * _FresnelColor.rgb;;
   FragColor = vec4(lighting, rimBloom);
//"   FragColor = vec4(1.0);
//"   FragColor = vec4(mix(_AmbientColor, _LightColor, shadowStep), 1.0);
//"   FragColor = vec4(specular, 1.0);
//"   if(ilmTex.b == 0) FragColor = vec4(0.5, 0, 0, 1.0);
}