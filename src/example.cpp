#include "example.h"

ExampleApplication::ExampleApplication() : nanogui::Screen(Eigen::Vector2i(1920, 1080), "Sora PathTracer", true) {
    using namespace nanogui;

    config = new Config(1, DIFFUSE, 1, 0.2, SAH, OPENGL);
    camera = new Camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.f, 0.f, 0.f), 1.0f);

    FormHelper* form = new FormHelper(this);
    ref<Window> setting = form->addWindow(Vector2i(MARGIN, MARGIN), "Settings");
    setting->setFixedWidth(FORM_WIDTH);
    setting->setFixedHeight(FORM_HEIGHT);

    // 渲染设置
    form->addGroup("Rendering Settings");
    // 渲染方式
    auto vRenderMode = form->addVariable("RenderMode", config->renderMode);
    vRenderMode->setFixedWidth(VALUE_WIDTH);
    vRenderMode->setItems({ "Naive", "OpenGL" });
    vRenderMode->setTooltip("if choose OpenGL, the OpenGL environment is needed.");

    // 可选择的模型

    // 采样次数
    auto vSampleCount = form->addVariable("SPP", config->sampleCount);
    vSampleCount->setFixedWidth(VALUE_WIDTH);
    vSampleCount->setSpinnable(true);
    vSampleCount->setFormat("[1-9][0-9]*");
    vSampleCount->setMinMaxValues(1, 2048);

    // 渲染材质
    auto vMaterial = form->addVariable("Material", config->material, true);
    vMaterial->setFixedWidth(VALUE_WIDTH);
    vMaterial->setItems({ "Diffuse", "Mirror", "Microfacet" });
    vMaterial->setTooltip("if set to Diffuse, the Roughness will be ignored; if set to Microfacet, the Sample Way will be ignored.");

    // 粗糙度
    auto vRoughness = form->addVariable("Roughness", config->roughness, false);
    vRoughness->setFixedWidth(VALUE_WIDTH);
    vRoughness->setSpinnable(true);
    vRoughness->setMinMaxValues(0.0, 1.0);
    vRoughness->setValueIncrement(0.01);
    vRoughness->setTooltip("Roughness value for microfacet material");

    // 优化设置
    form->addGroup("Optimization Settings");
    // 线程数
    auto vThreadCount = form->addVariable("Thread Count", config->threadCount, true);
    vThreadCount->setFixedWidth(VALUE_WIDTH);
    vThreadCount->setSpinnable(true);
    vThreadCount->setFormat("[1-9][0-9]*");
    vThreadCount->setMinMaxValues(1, 16);

    // 加速结构
    auto vAccelStructure = form->addVariable("Accel Strcuture", config->accelStructure, true);
    vAccelStructure->setFixedWidth(VALUE_WIDTH);
    vAccelStructure->setItems({ "None", "BVH", "SAH" });


    // 相机设置
    form->addGroup("Camera Settings");
    auto vOriginPositionX = form->addVariable("originPosition.X", camera->cameraPosition.x, true);
    vOriginPositionX->setFixedWidth(VALUE_WIDTH);
    vOriginPositionX->setSpinnable(true);

    auto vOriginPositionY = form->addVariable("originPosition.Y", camera->cameraPosition.y, true);
    vOriginPositionY->setFixedWidth(VALUE_WIDTH);
    vOriginPositionY->setSpinnable(true);

    auto vOriginPositionZ = form->addVariable("originPosition.Z", camera->cameraPosition.z, true);
    vOriginPositionZ->setFixedWidth(VALUE_WIDTH);
    vOriginPositionZ->setSpinnable(true);

    auto vOriginTargetX = form->addVariable("originTarget.X", camera->cameraTarget.x, true);
    vOriginTargetX->setFixedWidth(VALUE_WIDTH);
    vOriginTargetX->setSpinnable(true);

    auto vOriginTargetY = form->addVariable("originTarget.Y", camera->cameraTarget.y, true);
    vOriginTargetY->setFixedWidth(VALUE_WIDTH);
    vOriginTargetY->setSpinnable(true);

    auto vOriginTargetZ = form->addVariable("originTarget.Z", camera->cameraTarget.z, true);
    vOriginTargetZ->setFixedWidth(VALUE_WIDTH);
    vOriginTargetZ->setSpinnable(true);

    auto vAspect = form->addVariable("aspect", camera->aspect, true);
    vAspect->setFixedWidth(VALUE_WIDTH);
    vAspect->setSpinnable(true);
    vAspect->setMinValue(0.1f);

    // 已添加的模型列表
    Window* renderWindow = new Window(this, "Render Result");
    renderWindow->setPosition(Vector2i(FORM_WIDTH + 2 * MARGIN, MARGIN));
    renderWindow->setLayout(new GroupLayout());

    mCanvas = new MyGLCanvas(renderWindow, camera);
    mCanvas->setBackgroundColor({ 100, 100, 100, 255 });
    mCanvas->setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

    FormHelper* form1 = new FormHelper(this);
    ref<Window> setting1 = form1->addWindow(Vector2i(FORM_WIDTH + 5 * MARGIN + SCREEN_WIDTH, MARGIN), "Settings1");
    setting->setFixedWidth(FORM_WIDTH);
    setting->setFixedHeight(FORM_HEIGHT);

    // 模型设置
    form1->addGroup("Model Settings");
    // 缩放
    auto vScaleX = form1->addVariable("Scale.X", mCanvas->scale.x, true);
    vScaleX->setFixedWidth(VALUE_WIDTH);
    vScaleX->setSpinnable(true);
    vScaleX->setMinValue(0.1f);
    vScaleX->setValueIncrement(0.1f);

    auto vScaleY = form1->addVariable("Scale.Y", mCanvas->scale.y, true);
    vScaleY->setFixedWidth(VALUE_WIDTH);
    vScaleY->setSpinnable(true);
    vScaleY->setMinValue(0.1f);
    vScaleY->setValueIncrement(0.1f);

    auto vScaleZ = form1->addVariable("Scale.Z", mCanvas->scale.z, true);
    vScaleZ->setFixedWidth(VALUE_WIDTH);
    vScaleZ->setSpinnable(true);
    vScaleZ->setMinValue(0.1f);
    vScaleZ->setValueIncrement(0.1f);

    // 平移
    auto vTranslationX = form1->addVariable("Translation.X", mCanvas->translate.x, true);
    vTranslationX->setFixedWidth(VALUE_WIDTH);
    vTranslationX->setSpinnable(true);
    vTranslationX->setValueIncrement(1.f);

    auto vTranslationY = form1->addVariable("Translation.Y", mCanvas->translate.y, true);
    vTranslationY->setFixedWidth(VALUE_WIDTH);
    vTranslationY->setSpinnable(true);
    vTranslationY->setValueIncrement(1.f);

    auto vTranslationZ = form1->addVariable("Translation.Z", mCanvas->translate.z, true);
    vTranslationZ->setFixedWidth(VALUE_WIDTH);
    vTranslationZ->setSpinnable(true);
    vTranslationZ->setValueIncrement(1.f);
    // 旋转
    //auto vRotateX = form->addVariable("Rotate.X", true);
    //auto vRotateY = form->addVariable("Rotate.Y", true);
    //auto vRotateZ = form->addVariable("Rotate.Z", true);

    performLayout();
}

bool ExampleApplication::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers)) {
        return true;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return true;
    }
    return false;
}

void ExampleApplication::draw(NVGcontext* ctx) {
    /* Draw the user interface */
    Screen::draw(ctx);
}