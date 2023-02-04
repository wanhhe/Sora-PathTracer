#include "npr_shader.h"
#include "stb_image.h"

const string testhairShadowVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec4 FragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   FragPos = gl_Position;\n"
"}\n";

const string testhairShadowFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 FragPos;\n"
"uniform vec3 color;\n"
"void main() {\n"
//"   FragColor = vec4(color, 1.0);\n"
//"   FragColor = vec4(1.0);\n"
"   float depth = FragPos.z / FragPos.w;\n"
"   depth = depth * 0.5 + 0.5;\n"
"   if (depth < 0.9) FragColor = vec4(1.0,0,0,1.0);\n"
"   if (depth > 0.9) FragColor = vec4(0,1.0,0,1.0);\n"
//"   FragColor = vec4(depth, 0.0, 0.0, 1.0);\n"
"}\n";

const string faceVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec3 FragPos;\n"
"out vec4 position;\n"
"out vec3 Normal;\n"
"out vec2 TexCoords;\n"
"out vec3 viewLight;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec3 aLightPos;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = transpose(inverse(mat3(model))) * aNormal;\n"
"   TexCoords = aTexCoords;\n"
"   position = projection * view * vec4(FragPos, 1.0);\n"
"   viewLight = vec3(view * vec4(aLightPos, 1.0));\n"
"   gl_Position = position;\n"
"}\n";

const string faceFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoords;\n"
"in vec4 position;\n"
"in vec3 viewLight;\n"
"uniform sampler2D ilmTexture;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform sampler2D sdfMap;\n"
"uniform sampler2D hairLightTexture;\n"
"uniform sampler2D hairShadowMap;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 _MainColor;\n"
"uniform vec3 _ShadowColor;\n"
"uniform float _ShadowRange;\n"
"uniform float _ShadowSmooth;\n"
"uniform vec3 _SpecularColor;\n"
"uniform float _SpecularRange;\n"
"uniform float _SpecularMulti;\n"
"uniform float _SpecularGloss;\n"
"uniform vec3 _LightColor0;\n"
"uniform vec3 _FresnelColor;\n"
"uniform float _FresnelEff;\n"
"uniform float _HairShadowDistance;\n"
"uniform bool _isFace;\n"
"uniform bool _isHair;\n"
"vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
"   return F0 +(1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
"}\n"
"void main() {\n"
"   vec3 diffuseTex = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 ilmTex = texture(ilmTexture, TexCoords).rgb;\n"
"   vec3 normal = normalize(Normal);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 halfDir = normalize(lightDir + viewDir);\n"
"   float NdotH = max(dot(normal, halfDir), 0.0);\n"
"   float halfLambert = dot(normal, lightDir) * 0.5 + 0.5;\n"
"   float threshold = (halfLambert + ilmTex.g) * 0.5;\n"
"   float ramp = smoothstep(0, _ShadowSmooth, _ShadowRange - halfLambert);\n"
"   vec3 diffuse = mix(_ShadowColor, _MainColor, ramp);\n"
"   diffuse *= diffuseTex;\n"
"   vec3 specular = vec3(0.0);\n"
"   float specularSize = pow(NdotH, _SpecularGloss);\n"
"   float specularMark = ilmTex.b;\n"
"   if (specularSize >= 1 - specularMark * _SpecularRange) {\n"
"       specular = _SpecularMulti * ilmTex.r * _SpecularColor;\n"
"   }\n"
"   vec3 fresnel = _FresnelEff * fresnelSchlick(max(dot(viewDir, halfDir), 0.0), vec3(0.04));\n"
"   vec3 color = _LightColor0 * (diffuse + specular) + fresnel * _FresnelColor;\n"
//"   if (_isFace) {\n"
//"       vec2 scrPos = vec2(gl_FragCoord.x / 640.0, gl_FragCoord.y / 640.0);\n"
////"       vec2 scrPos = vec2(position.x / position.w, position.y / position.w);\n"
//"       vec3 gaga = normalize(viewLight);\n"
//"       float depth = (position.z / position.w) * 0.5 + 0.5;\n"
////"       vec2 samplingPoint = scrPos + _HairShadowDistance * gaga.xy * vec2(1.0/640.0, 1.0/640.0);\n"
//"       vec2 samplingPoint = scrPos + _HairShadowDistance * gaga.xy;\n"
//"       float hairShadow = texture(hairShadowMap, samplingPoint).r;\n"
//"       float inshadow = depth < hairShadow+0.0001 ? 0 : 1;\n"
//"       color *= inshadow;\n"
////"       if (hairShadow == 0.0) color = vec3(0, 0, 1.0);\n"
////"       if (hairShadow == 1.0) color = vec3(0, 1.0, 0);\n"
//"       if (depth < hairShadow) color = vec3(0, 1.0, 0);\n"
//"   }\n"
"   FragColor = vec4(color, 1.0);\n"
//"   float hairShadow = 1 - texture(hairShadowMap,vec2(0.0, 0.0)).r;\n"
//"   if (hairShadow == 0.0) FragColor = vec4(1.0,0,0,1.0);\n"
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
    lightPos = vec3(0 ,0, -3);
    lightColor = vec3(1, 0.792, 0.749);
    ambientColor = vec3(0.545, 0.490, 0.431);


    shader.initFromFiles("npr_shader", "..\\shaders\\npr\\normal.vs", "..\\shaders\\npr\\npr1.fs");
    //shader.init("npr_shader", nprVertexShader, nprFragShader);
    shader.bind();
    mat4 model(1.0);
    model = glm::translate(model, vec3(0.0));
    model = glm::scale(model, vec3(0.1));
    //model = glm::rotate(model, glm::radians(180.0f), vec3(0,1,0));
    shader.setUniform("model", model);
    shader.setUniform("_AmbientColor", ambientColor);
    shader.setUniform("_LightColor", lightColor);
    shader.setUniform("_SpecularColor", vec3(0.9));
    shader.setUniform("_SpecularRange", 0.7);
    shader.setUniform("ilmTexture", 10);
    shader.setUniform("sdfMap", 11);
    shader.setUniform("_RimColor", vec3(0.949, 0.624, 0.247));
    shader.setUniform("_RimAmount", 0.9);
    shader.setUniform("_MinRim", 0.65);
    shader.setUniform("_MaxRim", 0.8);


    shader2.initFromFiles("npr_shader2", "..\\shaders\\npr\\normal.vs", "..\\shaders\\npr\\npr2.fs");
    shader2.bind();
    shader2.setUniform("ilmTexture", 10);
    shader2.setUniform("sdfMap", 11);
    shader2.setUniform("hairLightTexture", 12);

    shader2.setUniform("lightPos", lightPos);
    shader2.setUniform("_AmbientColor", ambientColor);
    shader2.setUniform("_LightColor", lightColor);
    shader2.setUniform("_SpecularColor", vec3(0.9));
    shader2.setUniform("_SpecularRange", 0.7);
    shader2.setUniform("_RimColor", vec3(0.949, 0.624, 0.247));
    shader2.setUniform("_RimAmount", 0.9);
    shader2.setUniform("_MinRim", 0.65);
    shader2.setUniform("_MaxRim", 0.8);
    shader2.setUniform("_Bloom", 15.0);
    shader2.setUniform("_RimBloomExp", 4.0);
    shader2.setUniform("_ShadowAttWeight", 0.3);
    shader2.setUniform("_Atten", 0.3);
    shader2.setUniform("_DividLineH", 0.6);
    shader2.setUniform("_DividLineM", 0.0);
    shader2.setUniform("_DividLineD", -0.5);
    shader2.setUniform("_BoundSharp", 0.6);
    shader2.setUniform("model", model);


    shader3.initFromFiles("pnr_shader3", "..\\shaders\\npr\\normal.vs", "..\\shaders\\npr\\npr3.fs");
    shader3.bind();
    shader3.setUniform("lightPos", lightPos);
    shader3.setUniform("_AmbientColor", ambientColor);
    shader3.setUniform("_LightColor", lightColor);
    shader3.setUniform("_SpecularRange", 0.7);
    shader3.setUniform("ilmTexture", 10);
    shader3.setUniform("sdfMap", 11);
    shader3.setUniform("hairLightTexture", 12);
    shader3.setUniform("_RimColor", vec3(0.949, 0.624, 0.247));
    shader3.setUniform("_RimAmount", 0.9);
    shader3.setUniform("_MinRim", 0.65);
    shader3.setUniform("_MaxRim", 0.8);
    shader3.setUniform("_Bloom", 15.0);
    shader3.setUniform("_RimBloomExp", 4.0);
    shader3.setUniform("_ShadowAttWeight", 0.3);
    shader3.setUniform("_Atten", 0.3);
    shader3.setUniform("_DividSharpness", 1.5);
    shader3.setUniform("_DividLineSpec", 0.8);
    shader3.setUniform("_Glossiness", 0.8);
    shader3.setUniform("_AOWeight", 0.6);
    shader3.setUniform("_DividLineM", 0.0);
    shader3.setUniform("_DividLineD", -0.5);
    shader3.setUniform("_DarkFaceColor", vec3(0.851, 0.769, 0.671));
    shader3.setUniform("_DeepDarkColor", vec3(0.549, 0.498, 0.827));
    shader3.setUniform("_DiffuseBright", vec3(1.0));
    shader3.setUniform("_FresnelEff", 0.8);
    shader3.setUniform("_FresnelColor", vec3(0.3, 0, 0));
    shader3.setUniform("model", model);


    shader4.initFromFiles("npr_shader4", "..\\shaders\\npr\\normal.vs", "..\\shaders\\npr\\npr4.fs");
    shader4.bind();
    shader4.setUniform("model", model);
    shader4.setUniform("lightPos", lightPos);
    shader4.setUniform("ilmTexture", 10);
    shader4.setUniform("_MainColor", vec3(1.0));
    shader4.setUniform("_ShadowColor", vec3(0.7, 0.7, 0.8));
    shader4.setUniform("_ShadowRange", 0.5);
    shader4.setUniform("_ShadowSmooth", 0.2);
    //shader4.setUniform("_LightColor0", vec3(1.0, 0.792, 0.749));
    shader4.setUniform("_LightColor0", vec3(1.0));
    shader4.setUniform("_SpecularColor", vec3(1.0));
    shader4.setUniform("_SpecularRange", 0.7);
    shader4.setUniform("_SpecularMulti", 0.3);
    shader4.setUniform("_SpecularGloss", 16.0);
    shader4.setUniform("_FresnelEff", 0.8);
    shader4.setUniform("_FresnelColor", vec3(0.3, 0, 0));

    // 略微放大模型，存入缓冲
    outlineShader.initFromFiles("outline_color_buffer_shader", "..\\shaders\\npr\\outline.vs", "..\\shaders\\npr\\outline.fs");
    outlineShader.bind();
    outlineShader.setUniform("model", model);
    outlineShader.setUniform("outlineWidth", 2.0);
    //outlineShader.setUniform("outlineColor", vec3(0.392, 0.584, 0.929));
    outlineShader.setUniform("outlineColor", vec3(0.4));
    // 应该放入绘图函数中，为了简便暂不放入
    outlineShader.setUniform("aspect", 1.0);

    //lastFaceShader.init("hair_draw_shadow", faceVertexShader, faceFragShader);
    //lastFaceShader.bind();
    //lastFaceShader.setUniform("model", model);
    //lastFaceShader.setUniform("lightPos", lightPos);
    //lastFaceShader.setUniform("aLightPos", lightPos);
    //lastFaceShader.setUniform("ilmTexture", 10);
    //lastFaceShader.setUniform("sdfMap", 11);
    //lastFaceShader.setUniform("hairLightTexture", 12);
    //lastFaceShader.setUniform("hairShadowMap", 8);
    //lastFaceShader.setUniform("_MainColor", vec3(1.0));
    //lastFaceShader.setUniform("_ShadowColor", vec3(0.7, 0.7, 0.8));
    //lastFaceShader.setUniform("_ShadowRange", 0.5);
    //lastFaceShader.setUniform("_ShadowSmooth", 0.2);
    ////shader.setUniform("_LightColor0", vec3(1.0, 0.792, 0.749));
    //lastFaceShader.setUniform("_LightColor0", vec3(1.0));
    //lastFaceShader.setUniform("_SpecularColor", vec3(1.0));
    //lastFaceShader.setUniform("_SpecularRange", 0.7);
    //lastFaceShader.setUniform("_SpecularMulti", 0.3);
    //lastFaceShader.setUniform("_SpecularGloss", 16.0);
    //lastFaceShader.setUniform("_FresnelEff", 0.8);
    //lastFaceShader.setUniform("_FresnelColor", vec3(0.3, 0, 0));
    //lastFaceShader.setUniform("_HairShadowDistance", 0.0);

    //hairShadowStencilShader.init("hair_shadow_by_stencil", hairShadowStencilVertexShader, hairShadowStencilFragShader);
    bangsShadowStencilShader.initFromFiles("bangs_shadow_stencil", "..\\shaders\\npr\\bangs_shadow_stencil.vs", "..\\shaders\\npr\\bangs_shadow_stencil.fs");
    bangsShadowStencilShader.bind();
    bangsShadowStencilShader.setUniform("model", model);
    bangsShadowStencilShader.setUniform("_Offset", 0.01);

    bangsShadowShader.initFromFiles("draw_face_again_after_writing_face_stencil", "..\\shaders\\npr\\bangs_shadow.vs", "..\\shaders\\npr\\bangs_shadow.fs");
    bangsShadowShader.bind();
    bangsShadowShader.setUniform("model", model);
    bangsShadowShader.setUniform("lightPos", lightPos);
    bangsShadowShader.setUniform("ilmTexture", 10);
    bangsShadowShader.setUniform("sdfMap", 11);
    bangsShadowShader.setUniform("hairLightTexture", 12);
    bangsShadowShader.setUniform("hairShadowMap", 8);
    bangsShadowShader.setUniform("_MainColor", vec3(1.0));
    bangsShadowShader.setUniform("_ShadowColor", vec3(0.7, 0.7, 0.8));
    bangsShadowShader.setUniform("_ShadowRange", 0.5);
    bangsShadowShader.setUniform("_ShadowSmooth", 0.2);
    //shader.setUniform("_LightColor0", vec3(1.0, 0.792, 0.749));
    bangsShadowShader.setUniform("_LightColor0", vec3(1.0));
    bangsShadowShader.setUniform("_SpecularColor", vec3(1.0));
    bangsShadowShader.setUniform("_SpecularRange", 0.7);
    bangsShadowShader.setUniform("_SpecularMulti", 0.3);
    bangsShadowShader.setUniform("_SpecularGloss", 16.0);
    bangsShadowShader.setUniform("_FresnelEff", 0.8);
    bangsShadowShader.setUniform("_FresnelColor", vec3(0.3, 0, 0));
    bangsShadowShader.setUniform("_HairShadowColor", vec3(0.6, 0.5, 0.5));

    //glGenFramebuffers(1, &fbo);
    //glGenRenderbuffers(1, &captureRBO);
    //glGenTextures(1, &tbo);
}

void NPRShader::draw(const mat4& view, const mat4& projection, const vec3& viewPos) {
    //shader.bind();
    //shader.setUniform("view", view);
    //shader.setUniform("projection", projection);
    //shader.setUniform("viewPos", viewPos);
    //glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 10.0, 0.0, 0.0);
    ////shader.setUniform("lightPos", newPos);
    //model->draw(shader);

    glEnable(GL_CULL_FACE); // 开启面剔除
    glCullFace(GL_FRONT); // 正面剔除
    outlineShader.bind();
    outlineShader.setUniform("view", view);
    outlineShader.setUniform("projection", projection);
    model->draw(outlineShader);
    glDisable(GL_CULL_FACE);

    // 思路：首先确定是不是要画脸，如果是的话写入模板值，不是的话跳过。等到所有mesh都被绘制完后再绘制一次头发（相当于绘制两遍），并且对头发进行偏移和深度测试。
    // 模板测试，先写入脸的模板值, 然后正常绘制
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glClear(GL_STENCIL_BUFFER_BIT);
    //lastFaceShader.bind();
    //lastFaceShader.setUniform("view", view);
    //lastFaceShader.setUniform("projection", projection);
    //lastFaceShader.setUniform("viewPos", viewPos);
    //glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 10.0, 0.0, 0.0);
    //lastFaceShader.setUniform("lightPos", newPos);
    //lastFaceShader.setUniform("aLightPos", newPos);
    //model->drawFaceStencil(lastFaceShader);
    shader4.bind();
    shader4.setUniform("view", view);
    shader4.setUniform("projection", projection);
    shader4.setUniform("viewPos", viewPos);
    glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 8.0, 0.0, 0.0);
    shader4.setUniform("lightPos", newPos);
    model->drawFaceStencil(shader4);
    //// 然后画头发的阴影，如果模板值正确就画上脸 (再标记模板值)
    bangsShadowStencilShader.bind();
    bangsShadowStencilShader.setUniform("view", view);
    bangsShadowStencilShader.setUniform("projection", projection);
    //glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 10.0, 0.0, 0.0);
    bangsShadowStencilShader.setUniform("aLightPos", newPos);
    model->drawHairShadowStencil(bangsShadowStencilShader);
    // 再次按头发阴影的模板值渲染面部
    bangsShadowShader.bind();
    bangsShadowShader.setUniform("view", view);
    bangsShadowShader.setUniform("projection", projection);
    bangsShadowShader.setUniform("viewPos", viewPos);
    bangsShadowShader.setUniform("lightPos", newPos);
    model->drawFaceAfterSampleHairShadowStencil(bangsShadowShader);
    glDisable(GL_STENCIL_TEST);

    // 帧缓冲
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //glActiveTexture(GL_TEXTURE8);
    //glBindTexture(GL_TEXTURE_2D, tbo);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 640, 640, 0, GL_RG, GL_FLOAT, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 640);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    //glViewport(0, 0, 640, 640);
    //glScissor(0, 0, 640, 640);
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //testhairShader.bind();
    //testhairShader.setUniform("view", view);
    //testhairShader.setUniform("projection", projection);
    //model->drawHairShadow(testhairShader);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glViewport(405, 115, 640, 640);
    //glScissor(405, 115, 640, 640);
    //glClearColor(0.4, 0.4, 0.4, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //lastFaceShader.bind();
    //lastFaceShader.setUniform("view", view);
    //lastFaceShader.setUniform("projection", projection);
    //lastFaceShader.setUniform("viewPos", viewPos);
    //glm::vec3 newPos = lightPos + glm::vec3(sin(glfwGetTime() * 0.4) * 10.0, 0.0, 0.0);
    //lastFaceShader.setUniform("lightPos", newPos);
    //lastFaceShader.setUniform("aLightPos", newPos);
    //model->draw(lastFaceShader);
}

void NPRShader::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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