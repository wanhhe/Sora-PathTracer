#ifndef PATHTRACER_CANVANS_H
#define PATHTRACER_CANVANS_H

#include <nanogui/glcanvas.h>
#include <nanogui/glutil.h>
#include "camera.h"
#include "model.h"
#include "light.h"
#include "npr_shader.h"


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
    string addModel(const string& path, const string& name);
    string addLight(const string& name, int type, vec3 _position, vec3 _color, vec3 _intensity);
    Light* findLight(const string& id);
    Model* findModel(const string& id);
    Light* firstLight();
    Model* firstModel();
    void preloadPbrMapBalls();
    void preloadKlaudiaValentz();
    void preloadDiffuseIrradiance();
    void preloadShadowMapping();
    void preloadPCSS();
    void preloadSDF();
    void preloadNPR();
    void generateShadowMap();
    void renderShadowScene(nanogui::GLShader& shader);

    vec3 translate;
    vec3 scale;
    PreLoadEnum preload;
    ShaderStyleEnum shaderStyle;

private:
    vector<nanogui::GLShader> shaderList;
    vector<nanogui::GLShader> lightShaderList;
    nanogui::GLShader lightShader;
    mat4 model;
    mat4 view;
    mat4 projection;
    Camera* camera;
    vector<Model*> modelList;
    vector<Light*> lightList;
    int untitleModel;
    int untitleLight;
    bool init;

    NPRShader nprShader;

    unsigned int loadTexture(char const* path);
    void renderSphere();
    void renderCube();
};

#endif // !PATHTRACER_CANVANS_H