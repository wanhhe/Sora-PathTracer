#include <nanogui/glcanvas.h>
#include <nanogui/glutil.h>
#include "camera.h"
#include "model.h"
#include "light.h"

const string PhongVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 0) in vec3 aNormal;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"{\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string PhongFragShader =
"#version 330 core\n"
"out vec3 FragColor;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 objectColor;\n"
"void main() {\n"
"   float ambientStrength = 0.1;\n"
"   vec3 ambient = ambientStrenght * lightColor;\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(Normal, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   float specularStrength = 0.5;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, Normal);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n";

const string BlinnPhongVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 0) in vec3 aNormal;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"{\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string BlinnPhongFragShader =
"#version 330 core\n"
"out vec3 FragColor;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 kd;\n"
"uniform vec3 ks;\n"
"uniform vec3 ka;\n"
"uniform vec3 intensity;\n"
"uniform vec3 ambLightIntensity;\n"
"uniform vec3 lightPos;\n"
"void main() {\n"
"   vec3 viewDir = viewPos - FragPos;\n"
"   vec3 lightDir = lightPos - FragPos;\n"
"   vec3 l = normalize(lightDir);\n"
"   vec3 v = normalize(viewDir);\n"
"   vec3 half = normalize(viewDir + lightDir);\n"
"   vec3 realIntensity = intensity / dot(lightDir, lightDir);\n"
"   vec3 ambient = ka * ambLightIntensity;\n"
"   vec3 diffuse = kd * realIntensity * max(dot(Normal, l), 0);\n"
"   vec3 specular = ks * realIntensity * pow(max(dot(Normal, half), 0), 32);\n"
"   vec3 result = ambient + diffuse + specular;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n";



class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget* parent, Camera* _camera);

    ~MyGLCanvas() {
        //mShader.free();
    }

    virtual void drawGL();

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

    // 移动鼠标滚轮缩放相机
    virtual bool scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) override;

    /// 拖动鼠标右键平移相机，拖动鼠标左键旋转相机
    virtual bool mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;

    virtual bool mouseButtonEvent(const nanogui::Vector2i& p, int button, bool down, int modifiers) override;

    virtual bool mouseMotionEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;

    vec2 getPosDelta(vec2 old_pos, vec2 new_pos);

    void updateCamera();

    vec3 translate;
    vec3 scale;

    string addModel(const string& path, const string& name);

    string addLight(const string& name, int type, vec3 _position, vec3 _color, vec3 _intensity);

    Light* findLight(const string& id);

    Model* findModel(const string& id);

    Light* firstLight();

    Model* firstModel();

private:
    vector<nanogui::GLShader> shaderList;
    vector<nanogui::GLShader> lightShaderList;
    //nanogui::GLShader mShader;
    nanogui::GLShader lightShader;
    mat4 model;
    mat4 view;
    mat4 projection;
    Camera* camera;
    vector<Model*> modelList;
    vector<Light*> lightList;
    int untitleModel;
    int untitleLight;
};