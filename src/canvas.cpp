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
    nanogui::GLCanvas(parent), camera(_camera), untitleModel(1), untitleLight(1), preload(SHADOWMAPPING), init(false), shaderStyle(SHADER_STYLE_NONE) {
    using namespace nanogui;

    //modelList.emplace_back(new Model("..\\models\\sara\\sara.obj", "Model 1"));

    // 0
    GLShader diffuseShder;
    diffuseShder.init("diffuse_shader", diffuseVertexShader, diffuseFragShader);
    diffuseShder.bind();
    shaderList.emplace_back(diffuseShder);

    // 1
    GLShader phongShader;
    phongShader.init("phong_shader", phongVertexShader, phongFragShader);
    phongShader.bind();
    phongShader.setUniform("viewPos", camera->position);
    phongShader.setUniform("lightPos", vec3(2.5, 2.5, 2.5));
    phongShader.setUniform("lightColor", vec3(1.0, 1.0, 1.0));
    shaderList.emplace_back(phongShader);

    // 2
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

    // 3
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

    // 4
    GLShader cooktorranceShader;
    cooktorranceShader.initFromFiles("pbr_shader", "..\\shaders\\normal.vs", "..\\shaders\\pbr.fs");
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

    // 5
    GLShader pbrMapShader;
    pbrMapShader.initFromFiles("pbr_map_shader", "..\\shaders\\normal.vs", "..\\shaders\\pbr_map.fs");
    shaderList.emplace_back(pbrMapShader);

    // 6
    GLShader cubeMapShader;
    cubeMapShader.initFromFiles("ibl_hdr_cube_map_shader", "..\\shaders\\cubemap.vs", "..\\shaders\\equirectangular_to_cubemap.fs");
    shaderList.emplace_back(cubeMapShader);

    // 7
    GLShader backgroundShader;
    backgroundShader.initFromFiles("simple_skybox_shader", "..\\shaders\\background.vs", "..\\shaders\\background.fs");
    shaderList.emplace_back(backgroundShader);

    // 8
    GLShader hdrboxShader;
    hdrboxShader.initFromFiles("hdr_box", "..\\shaders\\cube_map.vs", "..\\shaders\\equirectangular_to_cubemap.fs");
    shaderList.emplace_back(hdrboxShader);

    // 9
    GLShader irradianceConvolutionShader;
    irradianceConvolutionShader.initFromFiles("irradiance_convolution_shader", "..\\shaders\\cubemap.vs", "..\\shaders\\irradiance_convolution.fs");
    shaderList.emplace_back(irradianceConvolutionShader);

    // 10
    GLShader diffuseIBLShader;
    diffuseIBLShader.initFromFiles("diffuse_ibl_shader", "..\\shaders\\normal.vs", "..\\shaders\\ibl_diffuse.fs");
    diffuseIBLShader.bind();
    diffuseIBLShader.setUniform("ao", 1.0f);
    diffuseIBLShader.setUniform("albedo", vec3(0.5f, 0.0f, 0.0f));
    shaderList.emplace_back(diffuseIBLShader);


    mat4 lightProjection, lightView;
    mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

     // 11
    GLShader shadowDepthShader;
    shadowDepthShader.initFromFiles("shadow_map_depth_shader", "..\\shaders\\shadow_map_depth.vs", "..\\shaders\\shadow_map_depth.fs");
    // render scene from light's point of view
    shadowDepthShader.bind();
    shadowDepthShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    shaderList.emplace_back(shadowDepthShader);

    // 12
    GLShader shadowShader;
    shadowShader.initFromFiles("shadow_map_shader", "..\\shaders\\shadow_map.vs", "..\\shaders\\shadow_map.fs");
    shadowShader.bind();
    shadowShader.setUniform("lightPos", vec3(-2.0f, 4.0f, -1.0f));
    shadowShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    shaderList.emplace_back(shadowShader);

    // 13
    GLShader debugShadowShader;
    debugShadowShader.initFromFiles("shadow_debug_shader", "..\\shaders\\shadow_debug.vs", "..\\shaders\\shadow_debug.fs");
    debugShadowShader.bind();
    // 消去输出，需要再加
    //debugShadowShader.setUniform("near_plane", near_plane);
    //debugShadowShader.setUniform("far_plane", far_plane);
    debugShadowShader.setUniform("depthMap", 0);
    shaderList.emplace_back(debugShadowShader);

    // 14
    GLShader pcssShader;
    pcssShader.initFromFiles("pcss_shadow_shader", "..\\shaders\\pcss.vs", "..\\shaders\\pcss.fs");
    pcssShader.bind();
    pcssShader.setUniform("lightPos", vec3(-2.0f, 4.0f, -1.0f));
    pcssShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    shaderList.emplace_back(pcssShader);

    // 15
    GLShader prefilterShader;
    prefilterShader.initFromFiles("prefilter_color_shader", "..\\shaders\\cubemap.vs", "..\\shaders\\prefilter.fs");
    shaderList.emplace_back(prefilterShader);

    // 16
    GLShader brdfShader;
    brdfShader.initFromFiles("brdf_sample_shader", "..\\shaders\\brdf.vs", "..\\shaders\\brdf.fs");
    shaderList.emplace_back(brdfShader);

    // 17
    GLShader pbrShader;
    pbrShader.initFromFiles("ibl_shader", "..\\shaders\\normal.vs", "..\\shaders\\ibl.fs");
    shaderList.emplace_back(pbrShader);

    // 18
    GLShader pbrmapShader;
    pbrmapShader.initFromFiles("ibl_map_shader", "..\\shaders\\normal.vs", "..\\shaders\\ibl_map.fs");
    shaderList.emplace_back(pbrmapShader);
    

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
}

void MyGLCanvas::drawGL() {
    using namespace nanogui;
    if (preload == PRELOAD_NONE) {
        if (shaderStyle == NPR) {
            glEnable(GL_DEPTH_TEST);
            updateCamera();
            view = lookAt(camera->position, camera->target, camera->up);
            projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
            nprShader.draw(view, projection, camera->position);
        }
        else {
            if (modelList.size() == 0) return;
            glEnable(GL_DEPTH_TEST);
            updateCamera();
            view = lookAt(camera->position, camera->target, camera->up);
            projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
            std::cout << "gaa" << std::endl;
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
                modelList[i]->draw(shaderList[modelList[i]->shaderIndex]);
            }
        }
    }
    else if (preload == PBRMAPBALLS) {
        glEnable(GL_DEPTH_TEST);
        preloadPbrMapBalls();
    }
    else if (preload == DIFFUSEIBL) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        preloadDiffuseIBL();
    }
    else if (preload == SHADOWMAPPING) {
        glEnable(GL_DEPTH_TEST);
        preloadShadowMapping();
    }
    else if (preload == PCSS) {
        glEnable(GL_DEPTH_TEST);
        preloadPCSS();
    }
    else if (preload == IBL_BALLS) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        preloadIBLBalls();
    }
    else if (preload == IBL_MAPBALLS) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        preloadIBLMapBalls();
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

void MyGLCanvas::preloadNPR() {

}

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
unsigned int depthMapFBO;
unsigned int depthMap;
unsigned int woodTexture;
unsigned int planeVAO;
void MyGLCanvas::preloadShadowMapping() {
    if (!init) {
        // 加载地板
        float planeVertices[] = {
            // positions            // normals         // texcoords
             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
             25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
        };
        // plane VAO
        unsigned int planeVBO;
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
        // 加载纹理
        woodTexture = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_basecolor.png");

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        // 让光锥之外不是阴影
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // 将生成的深度纹理作为深度缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shaderList[12].bind();
        shaderList[12].setUniform("diffuseTexture", 0);
        shaderList[12].setUniform("shadowMap", 1);
        
        init = true;
    }
    // 首先渲染深度纹理
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glScissor(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    renderShadowScene(shaderList[11]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 重定义大小屏幕
    glViewport(405, 115, 640, 640);
    glScissor(405, 115, 640, 640);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
    shaderList[12].bind();
    shaderList[12].setUniform("view", view);
    shaderList[12].setUniform("projection", projection);
    shaderList[12].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderShadowScene(shaderList[12]);

    //shaderList[13].bind();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, depthMap);
}

void MyGLCanvas::preloadPCSS() {
    if (!init) {
        // 加载地板
        float planeVertices[] = {
            // positions            // normals         // texcoords
             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
             25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
        };
        // plane VAO
        unsigned int planeVBO;
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
        // 加载纹理
        woodTexture = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_basecolor.png");

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        // 让光锥之外不是阴影
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // 将生成的深度纹理作为深度缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shaderList[14].bind();
        shaderList[14].setUniform("diffuseTexture", 0);
        shaderList[14].setUniform("shadowMap", 1);

        init = true;
    }
    // 首先渲染深度纹理
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glScissor(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    renderShadowScene(shaderList[11]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 重定义大小屏幕
    glViewport(405, 115, 640, 640);
    glScissor(405, 115, 640, 640);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
    shaderList[14].bind();
    shaderList[14].setUniform("view", view);
    shaderList[14].setUniform("projection", projection);
    shaderList[14].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderShadowScene(shaderList[14]);
}

void MyGLCanvas::renderShadowScene(nanogui::GLShader& shader) {
    using namespace nanogui;
    shader.bind();
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setUniform("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setUniform("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setUniform("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setUniform("model", model);
    renderCube();
}

void MyGLCanvas::generateShadowMap() {
    if (!init) {   
        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // 将生成的深度纹理作为深度缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        init = true;
    }
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
void MyGLCanvas::preloadDiffuseIBL() {
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

    shaderList[10].bind();
    shaderList[10].setUniform("view", view);
    shaderList[10].setUniform("projection", projection);
    shaderList[10].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shaderList[10].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
        shaderList[10].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
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

unsigned int prefilterMap;
unsigned int brdfLUTTexture;

void MyGLCanvas::preloadIBLBalls() {
    if (!init) {
        shaderList[17].bind();
        shaderList[17].setUniform("irradianceMap", 0);
        shaderList[17].setUniform("prefilterMap", 1);
        shaderList[17].setUniform("brdfLUT", 2);
        shaderList[17].setUniform("albedo", vec3(0.5f, 0.0f, 0.0f));
        shaderList[17].setUniform("ao", 1.0f);

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

        //float* data2 = stbi_loadf("ibl_brdf_lut.png", &width, &height, &nrComponents, 0);
        //if (data2)
        //{
        //    glGenTextures(1, &brdfLUTTexture);
        //    glActiveTexture(GL_TEXTURE2);
        //    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data2);

        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //    stbi_image_free(data2);
        //    glActiveTexture(GL_TEXTURE0);
        //    std::cout << nrComponents << std::endl;
        //}
        //else
        //{
        //    std::cout << "Failed to load HDR image." << std::endl;
        //}

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
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

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

        // prefilter cubemap
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        shaderList[15].bind();
        shaderList[15].setUniform("environmentMap", 0);
        shaderList[15].setUniform("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);
            glScissor(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            shaderList[15].setUniform("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                shaderList[15].setUniform("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // brdf lut texture
        glGenTextures(1, &brdfLUTTexture);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
        glViewport(0, 0, 512, 512);
        glScissor(0, 0, 512, 512);
        shaderList[16].bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        glViewport(405, 115, 640, 640);
        glScissor(405, 115, 640, 640);
        init = true;
    }
    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);

    shaderList[17].bind();
    shaderList[17].setUniform("view", view);
    shaderList[17].setUniform("projection", projection);
    shaderList[17].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shaderList[17].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
        shaderList[17].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row)
    {
        shaderList[17].setUniform("metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col)
        {
            shaderList[17].setUniform("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                (float)(col - (nrColumns / 2)) * spacing,
                (float)(row - (nrRows / 2)) * spacing,
                -2.0f
            ));
            shaderList[17].setUniform("model", model);
            renderSphere();
        }
    }

    // render skybox (render as last to prevent overdraw)
    shaderList[7].bind();
    shaderList[7].setUniform("view", view);
    shaderList[7].setUniform("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
        //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
    renderCube();

    // render BRDF map to screen
        //shaderList[16].bind();
        //renderQuad();
}

unsigned int lightGoldAlbedoMap;
unsigned int lightGoldNormalMap;
unsigned int lightGoldMetallicMap;
unsigned int lightGoldRoughnessMap;
unsigned int lightGoldAOMap;
unsigned int grayAlbedoMap;
unsigned int grayNormalMap;
unsigned int grayMetallicMap;
unsigned int grayRoughnessMap;
unsigned int grayAOMap;
unsigned int goldScuffedAlbedoMap;
unsigned int goldScuffedNormalMap;
unsigned int goldScuffedMetallicMap;
unsigned int goldScuffedRoughnessMap;
unsigned int goldScuffedAOMap;
unsigned int rustedironAlbedoMap;
unsigned int rustedironNormalMap;
unsigned int rustedironMetallicMap;
unsigned int rustedironRoughnessMap;
unsigned int rustedironAOMap;
void MyGLCanvas::preloadIBLMapBalls() {
    if (!init) {
        shaderList[18].bind();
        shaderList[18].setUniform("irradianceMap", 0);
        shaderList[18].setUniform("prefilterMap", 1);
        shaderList[18].setUniform("brdfLUT", 2);
        shaderList[18].setUniform("albedoMap", 3);
        shaderList[18].setUniform("normalMap", 4);
        shaderList[18].setUniform("metallicMap", 5);
        shaderList[18].setUniform("roughnessMap", 6);
        shaderList[18].setUniform("aoMap", 7);

        shaderList[7].bind();
        shaderList[7].setUniform("environmentMap", 0);

        lightGoldAlbedoMap = loadTexture("..\\models\\light-gold-bl\\lightgold_albedo.png");
        lightGoldNormalMap = loadTexture("..\\models\\light-gold-bl\\lightgold_normal-ogl.png");
        lightGoldMetallicMap = loadTexture("..\\models\\light-gold-bl\\lightgold_metallic.png");
        lightGoldRoughnessMap = loadTexture("..\\models\\light-gold-bl\\lightgold_roughness.png");
        lightGoldAOMap = loadTexture("..\\models\\light-gold-bl\\lightgold_ambient.png");

        grayAlbedoMap = loadTexture("..\\models\\gray-granite-flecks-bl\\gray-granite-flecks-albedo.png");
        grayNormalMap = loadTexture("..\\models\\gray-granite-flecks-bl\\gray-granite-flecks-Normal-ogl.png");
        grayMetallicMap = loadTexture("..\\models\\gray-granite-flecks-bl\\gray-granite-flecks-Metallic.png");
        grayRoughnessMap = loadTexture("..\\models\\gray-granite-flecks-bl\\gray-granite-flecks-Roughness.png");
        grayAOMap = loadTexture("..\\models\\gray-granite-flecks-bl\\gray-granite-flecks-ao.png");

        goldScuffedAlbedoMap = loadTexture("..\\models\\gold-scuffed-bl\\gold-scuffed_basecolor.png");
        goldScuffedNormalMap = loadTexture("..\\models\\gold-scuffed-bl\\gold-scuffed_normal.png");
        goldScuffedMetallicMap = loadTexture("..\\models\\gold-scuffed-bl\\gold-scuffed_metallic.png");
        goldScuffedRoughnessMap = loadTexture("..\\models\\gold-scuffed-bl\\gold-scuffed_roughness.png");
        goldScuffedAOMap = loadTexture("..\\models\\gold-scuffed-bl\\gold-scuffed_ambient.png");

        rustedironAlbedoMap = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_basecolor.png");
        rustedironNormalMap = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_normal.png");
        rustedironMetallicMap = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_metallic.png");
        rustedironRoughnessMap = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_roughness.png");
        rustedironAOMap = loadTexture("..\\models\\rustediron1-alt2-bl\\rustediron2_ambient.png");

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
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

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

        // prefilter cubemap
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        shaderList[15].bind();
        shaderList[15].setUniform("environmentMap", 0);
        shaderList[15].setUniform("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);
            glScissor(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            shaderList[15].setUniform("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                shaderList[15].setUniform("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // brdf lut texture
        glGenTextures(1, &brdfLUTTexture);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
        glViewport(0, 0, 512, 512);
        glScissor(0, 0, 512, 512);
        shaderList[16].bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        glViewport(405, 115, 640, 640);
        glScissor(405, 115, 640, 640);
        init = true;
    }
    updateCamera();
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);

    shaderList[18].bind();
    shaderList[18].setUniform("view", view);
    shaderList[18].setUniform("projection", projection);
    shaderList[18].setUniform("viewPos", camera->position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, lightGoldAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, lightGoldNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, lightGoldMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, lightGoldRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, lightGoldAOMap);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0, 0.0, 2.0));
    shaderList[18].setUniform("model", model);
    renderSphere();

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, grayAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, grayNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, grayMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, grayRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, grayAOMap);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
    shaderList[18].setUniform("model", model);
    renderSphere();

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, goldScuffedAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, goldScuffedNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, goldScuffedMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, goldScuffedRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, goldScuffedAOMap);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0, 0.0, 2.0));
    shaderList[18].setUniform("model", model);
    renderSphere();

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, rustedironAlbedoMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, rustedironNormalMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, rustedironMetallicMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, rustedironRoughnessMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, rustedironAOMap);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
    shaderList[18].setUniform("model", model);
    renderSphere();

    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shaderList[18].setUniform("lightPosition[" + std::to_string(i) + "]", newPos);
        shaderList[18].setUniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }

    // render skybox (render as last to prevent overdraw)
    shaderList[7].bind();
    shaderList[7].setUniform("view", view);
    shaderList[7].setUniform("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
        //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
    renderCube();

    // render BRDF map to screen
        //shaderList[16].bind();
        //renderQuad();
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

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void MyGLCanvas::renderQuad()
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
    if (camera->isOrbiting) {
        vec2 posDelta = getPosDelta(camera->orbitPos, cursorPos);
        camera->orbitDelta += posDelta;
        camera->orbitPos = cursorPos;
    }
    if (camera->isPanning) {
        vec2 posDelta = getPosDelta(camera->panPos, cursorPos);
        camera->panDelta += posDelta;
        camera->panPos = cursorPos;
    }
    camera->orbit = camera->orbitDelta;
    camera->pan = camera->panDelta;
    camera->dolly = camera->dollyDelta;
    camera->updateTransform();
    
    camera->dollyDelta = 0;
    camera->orbitDelta.x = 0;
    camera->orbitDelta.y = 0;
    camera->panDelta.x = 0;
    camera->panDelta.y = 0;
}

bool MyGLCanvas::keyboardEvent(int key, int scancode, int action, int modifiers) {
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //    setVisible(false);
    //    return false;
    //}
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera->recoverDefaultLocation();
    }
    return true;
}