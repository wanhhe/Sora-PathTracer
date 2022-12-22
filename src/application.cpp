#include <thread>
#include <chrono>

#include "common.h"
#include "application.h"

Application::Application(): nanogui::Screen(nanogui::Vector2i(SCREEN_WIDTH + FORM_WIDTH + PAD_WIDTH + 3 * MARGIN, APP_HEIGHT + 2 * MARGIN),
                                            "PathTracer", false) {
    using namespace nanogui;

    config = new Config(1, DIFFUSE, 1, 0.2, SAH);

    FormHelper *form = new FormHelper(this);
    ref<Window> setting = form->addWindow(Vector2i(MARGIN, MARGIN), "Settings");
    setting->setFixedWidth(FORM_WIDTH);
    setting->setFixedHeight(FORM_HEIGHT);

    // 渲染设置
    form->addGroup("Rendering Settings");
    // 可选择的模型
    form->addWidget("test", new Widget(this));

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
    //form->addVariable("fov");
    //form->addVariable("aspecRatio");
    //form->addVariable("X");
    //form->addVariable("Y");
    //form->addVariable("Z");
    //form->addVariable("X");
    //form->addVariable("Y");
    //form->addVariable("Z");
    //form->addVariable("X");
    //form->addVariable("Y");
    //form->addVariable("Z");


    // 已添加的模型列表
    // 模型设置
    form->addGroup("Translation");
    //form->addVariable("X");
    //form->addVariable("Y");
    //form->addVariable("Z");

    form->addGroup("Scale");
    //form->addVariable("X");
    //form->addVariable("Y");
    //form->addVariable("Z");

    // time，用于计时


    start = form->addButton("Start", [this]() {
        this->run();
    });

    // 进度条
    auto progress = new Window(this, "Progress");
    progress->setPosition(Vector2i(MARGIN, FORM_HEIGHT + 2 * MARGIN));
    progress->setLayout(new GroupLayout());
    progress->setFixedWidth(FORM_WIDTH);

    progressBar = new ProgressBar(progress);
    progressBar->setValue(imageDataProgress);

    // 渲染结果
    image = new Window(this, "Rendering Result");
    image->setPosition(Vector2i(FORM_WIDTH + 2 * MARGIN, MARGIN));
    image->setLayout(new GroupLayout());
    image->setFixedWidth(SCREEN_WIDTH + PAD_WIDTH);
    image->setFixedHeight(SCREEN_HEIGHT + PAD_HEIGHT);

    showInit();
}

void Application::run() {
    std::thread threadRun([this]() {
        this->start->setEnabled(false);

        std::cout << "=========   Render Settings   =========" << std::endl
            << "Sample Count: " << this->config->sampleCount << std::endl
            << "Material: " << this->config->material << std::endl
            << "Roughness: " << this->config->roughness << std::endl
            << "Thread Count: " << this->config->threadCount << std::endl
            << "Accel Structure: " << this->config->accelStructure << std::endl;

        auto initStartTime = std::chrono::high_resolution_clock::now();

        auto initEndTime = std::chrono::high_resolution_clock::now();
        auto initDuration = std::chrono::duration_cast<std::chrono::milliseconds>(initEndTime - initStartTime);
        std::cout << "=========  Init Duration Time  =========" << std::endl
            << "Time: " << initDuration.count() << "ms" << std::endl << std::endl;

        this->start->setEnabled(true);
        });
    std::cout << "run!!" << std::endl;
}

void Application::showInit() {
    using namespace nanogui;

    performLayout();
}

void Application::drawAll() {
    if (imageDataSignal) {
        progressBar->setValue(imageDataProgress);
        imageDataSignal = false;
    }
    Screen::drawAll();
}

void Application::readModel() {

}

void Application::loadModel() {

}