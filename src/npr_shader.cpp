#include "npr_shader.h"
#include "stb_image.h"

const string nprVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 TexCoords;\n"
"out vec4 lightFragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform mat4 lightSpaceMatrix;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = transpose(inverse(mat3(model))) * aNormal;\n"
"   TexCoords = aTexCoords;\n"
"   lightFragPos = lightSpaceMatrix * vec4(FragPos, 1.0);\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string nprFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoords;\n"
"in vec4 lightFragPos;\n"
"uniform sampler2D ilmTexture;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform sampler2D sdfMap;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 ambientColor;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 specularColor;\n"
"uniform float _SpecularRange;\n"
"uniform vec3 _RimColor;\n"
"uniform float _RimAmount;\n"
"uniform float _minRim;\n"
"uniform float _maxRim;\n"
"void main() {\n"
"   vec3 color = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 ilmTex = texture(ilmTexture, TexCoords).rgb;\n"
"   vec3 normal = normalize(Normal);\n"
"   if(!gl_FrontFacing) normal = -normal;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float NdotL = max(dot(normal, lightDir), 0);\n"
"   float wrapLambert = NdotL + ilmTex.g;\n"
"   float shadowStep = smoothstep(0.9, 0.95, wrapLambert);\n"
"   vec3 shadow = mix(ambientColor, lightColor, shadowStep);\n"
"   vec3 halfwayDir = normalize(lightDir + viewDir);\n"
"   float spec = pow(max(dot(normal, halfwayDir), 0), 128);\n"
"   float specularRange = step(_SpecularRange, (spec + clamp(dot(normal.xz, lightDir.xz), 0.0, 1.0)) * ilmTex.r * ilmTex.b);\n"
"   vec3 specular = specularRange * specularColor;\n"
"   vec3 Front = vec3(0, 0, -1.0);\n"
"   float rimDot = 1 - clamp(dot(viewDir, normal), 0.0, 1.0);\n"
"   rimDot = smoothstep(_minRim, _maxRim, rimDot);\n"
"   float rimIntensity = rimDot * clamp(dot(-Front.xz, lightDir.xz), 0.0, 1.0);\n"
"   rimIntensity = smoothstep(_RimAmount - 0.3, _RimAmount + 0.1, rimIntensity);\n"
"   if (dot(normal, Front) < 0) rimIntensity = 0.0;\n"
//"   vec3 rimColor = rimIntensity * lightColor;\n"
"   vec3 rimColor = rimIntensity * _RimColor;\n"
"   vec4 sdfIlm = texture(sdfMap, TexCoords);\n"
"   vec4 r_sdfIlm = texture(sdfMap, vec2(1 - TexCoords.x, TexCoords.y));\n"
"   vec3 Left = normalize(vec3(1,0,0));\n"
"   vec3 Right = -Left;\n"
"   float ctrl = step(0, dot(Front.xz, lightDir.xz));\n"
"   float faceShadow = ctrl * min(step(dot(Left.xz, lightDir.xz), r_sdfIlm.r), step(dot(Right.xz, lightDir.xz), sdfIlm.r));\n"
"   vec3 diffuse = mix(ambientColor, lightColor, faceShadow);\n"
//"   float ctrl = 1 - clamp(dot(Front, lightDir) * 0.5 + 0.5, 0.0, 1.0);\n"
//"   float ilm = dot(lightDir, Left) > 0 ? sdfIlm.r : r_sdfIlm.r;\n"
//"   float isShadow = step(ilm, ctrl);\n"
//"   float bias = smoothstep(0.6, 0.8, abs(ctrl - ilm));\n"
//"   vec3 diffuse = color;\n"
//"   if (ctrl > 0.99 || isShadow == 1) diffuse = mix(lightColor, ambientColor ,bias);\n"
//"   vec3 lighting = rimColor + (shadow + specular) * color;\n"
"   vec3 lighting = (rimColor + diffuse + specular) * color;\n"
"   FragColor = vec4(lighting , 1.0);\n"
//"   FragColor = vec4(mix(ambientColor, lightColor, shadowStep), 1.0);\n"
//"   FragColor = vec4(specular, 1.0);\n"
//"   if (spec == 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (ilmTex.r == 0 || ilmTex.b == 0) FragColor = vec4(0, 0.5, 0, 1.0);\n"
//"   if (rimColor.x > 0.2 || rimColor.y > 0.2 || rimColor.z > 0.2) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (NdotL > 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if(!gl_FrontFacing) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (dot(vec2(0, 1), lightDir.xz) < 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (sdfIlm.x > 0.7) FragColor = vec4(0.5, 0, 0, 1.0);\n"
"}\n";

const string testFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoords;\n"
"in vec4 lightFragPos;\n"
"uniform sampler2D ilmTexture;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform sampler2D sdfMap;\n"
"uniform sampler2D hairLightTexture;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 ambientColor;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 specularColor;\n"
"uniform float _SpecularRange;\n"
"uniform vec3 _RimColor;\n"
"uniform float _RimAmount;\n"
"uniform float _minRim;\n"
"uniform float _maxRim;\n"
"uniform float fa;\n"
"uniform float _RimBloomExp;\n"
"void main() {\n"
"   vec3 color = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 ilmTex = texture(ilmTexture, TexCoords).rgb;\n"
"   vec3 normal = normalize(Normal);\n"
"   if(!gl_FrontFacing) normal = -normal;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float NdotL = max(dot(normal, lightDir), 0);\n"
"   float wrapLambert = NdotL + ilmTex.g;\n"
"   float shadowStep = smoothstep(0.9, 0.95, wrapLambert);\n"
"   vec3 shadow = mix(ambientColor, lightColor, shadowStep);\n"
"   vec3 halfwayDir = normalize(lightDir + viewDir);\n"
"   float spec = pow(max(dot(normal, halfwayDir), 0), 128);\n"
"   float specularRange = step(_SpecularRange, (spec + clamp(dot(normal.xz, lightDir.xz), 0.0, 1.0)) * ilmTex.r * ilmTex.b);\n"
"   vec3 specular = specularRange * specularColor;\n"
"   vec3 Front = vec3(0, 0, -1.0);\n"
"   float f = 1.0 - clamp(dot(viewDir, normal), 0.0, 1.0);\n"
"   float rim = smoothstep(_minRim, _maxRim, f);\n"
"   rim = smoothstep(0, _RimAmount, rim);\n"
// 使用mask控制bloom区域
"   vec3 rimColor = texture(hairLightTexture, TexCoords).g * rim * _RimColor.rgb;\n"
"   float rimBloom;\n"
// 背光直接是1， 不背光
"   if (dot(normal, lightDir) < 0.2 || f < 0.6) {\n"
"       rimBloom = 1.0;\n"
"   } else {\n"
"       rimBloom = pow(f, _RimBloomExp) * fa * dot(normal, lightDir);\n"
"   }\n"
"   vec4 sdfIlm = texture(sdfMap, TexCoords);\n"
"   vec4 r_sdfIlm = texture(sdfMap, vec2(1 - TexCoords.x, TexCoords.y));\n"
"   vec3 Left = normalize(vec3(1,0,0));\n"
"   vec3 Right = -Left;\n"
"   float ctrl = step(0, dot(Front.xz, lightDir.xz));\n"
"   float faceShadow = ctrl * min(step(dot(Left.xz, lightDir.xz), r_sdfIlm.r), step(dot(Right.xz, lightDir.xz), sdfIlm.r));\n"
"   vec3 diffuse = mix(ambientColor, lightColor, faceShadow);\n"
//"   float ctrl = 1 - clamp(dot(Front, lightDir) * 0.5 + 0.5, 0.0, 1.0);\n"
//"   float ilm = dot(lightDir, Left) > 0 ? sdfIlm.r : r_sdfIlm.r;\n"
//"   float isShadow = step(ilm, ctrl);\n"
//"   float bias = smoothstep(0.6, 0.8, abs(ctrl - ilm));\n"
//"   vec3 diffuse = color;\n"
//"   if (ctrl > 0.99 || isShadow == 1) diffuse = mix(lightColor, ambientColor ,bias);\n"
//"   vec3 lighting = rimColor + (shadow + specular) * color;\n"
"   vec3 lighting = (rimColor + diffuse + specular) * color;\n"
//"   FragColor = vec4(lighting, rimBloom);\n"
"   FragColor = vec4(0.0, 0.5, 0.0, 1.0);\n"
//"   FragColor = vec4(mix(ambientColor, lightColor, shadowStep), 1.0);\n"
//"   FragColor = vec4(specular, 1.0);\n"
//"   if (spec == 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (ilmTex.r == 0 || ilmTex.b == 0) FragColor = vec4(0, 0.5, 0, 1.0);\n"
//"   if (rimColor.x > 0.2 || rimColor.y > 0.2 || rimColor.z > 0.2) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (NdotL > 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if(!gl_FrontFacing) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (dot(vec2(0, 1), lightDir.xz) < 0) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (sdfIlm.x > 0.7) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (rimBloom < 1) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (pow(f, _RimBloomExp) < 0.2) FragColor = vec4(0.5, 0, 0, 1.0);\n"
//"   if (f > 0.6) FragColor = vec4(0, 0.5, 0, 1.0);\n"
"}\n";

const string outlineVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform float outlineWidth;\n"
"uniform float aspect;\n"
"void main() {\n"
"   vec4 FragPos = projection * view * model * vec4(aPos, 1.0);\n"
"   vec4 Normal = projection * view * vec4(normalize(transpose(inverse(mat3(model))) * aNormal), 0);\n"
"   Normal.x *= aspect;\n"
"   FragPos.xy += Normal.xy * FragPos.w * outlineWidth * 0.001\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position = FragPos;\n"
"}\n";

const string outlineFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main() {\n"
//"   FragColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"   FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);\n"
"}\n";

NPRShader::NPRShader() {
    model = new Model("..\\models\\hotaru\\hotaru.obj", "hotaru");
    bodyLightMap = loadTexture("..\\models\\hotaru\\Texture\\Avatar_Girl_Sword_PlayerGirl_Tex_Body_Lightmap.png");
    hairLightMap = loadTexture("..\\models\\hotaru\\Texture\\Avatar_Girl_Sword_PlayerGirl_Tex_Hair_Lightmap.png");
    hairSpecularMap = loadTexture("..\\models\\hotaru\\Texture\\sp.png");
    sdfMap = loadTexture("..\\models\\hotaru\\Texture\\wow.png");
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, bodyLightMap);
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, sdfMap);
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, hairLightMap);
    glActiveTexture(GL_TEXTURE0);
    //lightPos = vec3(2, 4, 0);
    lightPos = vec3(0, 4, -3);
    lightColor = vec3(1, 0.792, 0.749);
    ambientColor = vec3(0.545, 0.490, 0.431);
    shader.init("npr_shader", nprVertexShader, testFragShader);
    shader.bind();
    shader.setUniform("lightPos", lightPos);
    shader.setUniform("ambientColor", ambientColor);
    shader.setUniform("lightColor", lightColor);
    shader.setUniform("specularColor", vec3(0.9));
    shader.setUniform("_SpecularRange", 0.7);
    shader.setUniform("ilmTexture", 10);
    shader.setUniform("sdfMap", 11);
    shader.setUniform("hairLightTexture", 12);
    shader.setUniform("_RimColor", vec3(0.949, 0.624, 0.247));
    shader.setUniform("_RimAmount", 0.9);
    shader.setUniform("_minRim", 0.65);
    shader.setUniform("_maxRim", 0.8);
    shader.setUniform("fa", 15.0);
    shader.setUniform("_RimBloomExp", 4.0);
    mat4 model(1.0);
    model = glm::translate(model, vec3(0.0));
    model = glm::scale(model, vec3(0.2));
    shader.setUniform("model", model);

    // 略微放大模型，存入缓冲
    colorBufferShader.init("outline_color_buffer_shader", outlineVertexShader, outlineFragShader);
    colorBufferShader.bind();
    colorBufferShader.setUniform("model", model);
    colorBufferShader.setUniform("outlineWidth", 1.5);
    // 应该放入绘图函数中，为了简便暂不放入
    colorBufferShader.setUniform("aspect", 1.0);
}

void NPRShader::draw(const mat4& view, const mat4& projection, const vec3& viewPos) {
    shader.bind();
    shader.setUniform("view", view);
    shader.setUniform("projection", projection);
    shader.setUniform("viewPos", viewPos);
    //glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 10.0, 0.0, 0.0);
    //shader.setUniform("lightPos", newPos);
    model->draw(shader);

    glEnable(GL_CULL_FACE); // 开启面剔除
    glCullFace(GL_FRONT); // 正面剔除
    colorBufferShader.bind();
    colorBufferShader.setUniform("view", view);
    colorBufferShader.setUniform("projection", projection);
    model->draw(colorBufferShader);
}

unsigned int NPRShader::loadTexture(char const* path)
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