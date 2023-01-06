#include "example.h"

void toggleAddModelWnd(ExampleApplication* _screen, bool show) {
    using namespace nanogui;

    auto model = _screen->findWidgetGlobal("#add_model_wnd");
    
    if (!model) {
        auto& wnd = _screen->window("Add Model");
        wnd.setPosition(60, 60);
        wnd.withLayout<GroupLayout>();
        wnd.setFixedSize({ 300, 300 });
        wnd.setId("#add_model_wnd");
        wnd.label(Caption{ "Model Path" });
        auto& text = wnd.textbox(IsEditable{ true });
        wnd.button(Caption{ "open" },
            FixedSize{ 60, 30 },
            ButtonCallback{ [&] {
                text.setValue(file_dialog({ { "png", "Portable Network Graphics" },{ "txt", "Text file" }, {"obj", "Object"}}, false));
            } });
        wnd.label(Caption{ "Model Name" });
        auto& name = wnd.textbox(IsEditable{ true });
        wnd.button(Caption{ "Create" },
            FixedSize{ 60, 30 },
            ButtonCallback{ [&, _screen] {
                string item = _screen->mCanvas->addModel(text.value(), name.value());
                Listbox* model_list = _screen->findWidget<Listbox>("#model_list");
                model_list->addItem(item, item);
                _screen->performLayout();
              wnd.dispose();
            } });
        wnd.button(Caption{ "Cancle" },
            FixedSize{ 60, 30 },
            ButtonCallback{ [&] {
              wnd.dispose();
            } });
        
        //elm::BeginWindow{ Caption{ "Add Model" }, Position{ 60, 60 },
        //            WidgetId{ "#add_model_wnd"}, WidgetGroupLayout{}, WindowMovable{ Theme::WindowDraggable::dgFixed } };
        // //模型名称
        //elm::Label{ Caption{ "Model Path" }, CaptionFont{ "sans-bold" } };
        //elm::Widget{
        //    WidgetBoxLayout{ Orientation::Horizontal, Alignment::Middle, 0, 6 },
        //    elm::Textbox{ IsEditable{true} },
        //    elm::Button{ Caption{ "Open" }, ButtonCallback{ [&] {
        //          cout << "File dialog result: "
        //            << file_dialog({ { "png", "Portable Network Graphics" },{ "txt", "Text file" }, {"obj", "Object"}}, false)
        //            << endl;
        //        }}
        //      },
        //                elm::Button{ Caption{ "Close" }, ButtonCallback{ [&] {
        //            if (screen->findWidgetGlobal("#add_model_wnd") == nullptr) std::cout << "ha" << std::endl;
        //           //screen->findWidgetGlobal("#add_model_wnd")->window()->dispose();
        //        }}
        //      }
        //};
        //elm::Label{ Caption{ "Model Name" }, CaptionFont{ "sans-bold" } };
        //elm::Textbox{ IsEditable{true}};
        //// 模型路径
        //elm::Button{ Caption{ "Open" }, ButtonCallback{ [&] {
        //      cout << "File dialog result: "
        //        << file_dialog({ { "png", "Portable Network Graphics" },{ "txt", "Text file" } }, false)
        //        << endl;
        //    }}
        //},
        //    elm::Label{ Caption{ "File dialog" }, CaptionFont{ "sans-bold" } };

        //    elm::Widget{
        //      WidgetBoxLayout{ Orientation::Horizontal, Alignment::Middle, 0, 6 },
        //      elm::Button{ Caption{ "Open" }, ButtonCallback{ [&] {
        //          cout << "File dialog result: "
        //            << file_dialog({ { "png", "Portable Network Graphics" },{ "txt", "Text file" } }, false)
        //            << endl;
        //        }}
        //      },
        //      elm::Button{ Caption{ "Save" }, ButtonCallback{ [&] {
        //          cout << "File dialog result: "
        //            << file_dialog({ { "png", "Portable Network Graphics" },{ "txt", "Text file" } }, true)
        //            << endl;
        //        }}
        //      }
        //    };
        //    elm::EndWindow{};
            _screen->performLayout();
    }
    else {
        model->setVisible(show);
    }
}

void toggleAddLightWnd(ExampleApplication* screen, bool show) {
    using namespace nanogui;

    auto light = screen->findWidgetGlobal("#add_light_wnd");
    if (!light) {
        auto& wnd = screen->window("Add Light");
        wnd.setPosition(60, 60);
        auto* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
        layout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
        layout->setSpacing(0, 10);
        wnd.setLayout(layout);
        //wnd.withLayout<GroupLayout>();
        wnd.setFixedSize({ 250, 370 });
        wnd.setId("#add_light_wnd");
        wnd.label(Caption{ "Light Name" });
        auto& name = wnd.textbox(IsEditable{ true });
        wnd.label(Caption{ "Light Type" });
        auto& type = wnd.combobox(ComboBoxItems{ "Ambient", "Point", "Flash" },
            FontSize{ 16 },
            FixedSize{ 100, 20 });
        wnd.label(Caption{ "Light Position X" });
        auto& positionX = wnd.floatbox<float>(IsEditable{ true });
        wnd.label(Caption{ "Light Position Y" });
        auto& positionY = wnd.floatbox<float>(IsEditable{ true });
        wnd.label(Caption{ "Light Position Z" });
        auto& positionZ = wnd.floatbox<float>(IsEditable{ true });
        //wnd.label(Caption{ "Light Color" });
        wnd.label("Red: ");
        auto& re = wnd.intbox<int>(IsEditable{ true });
        re.setSpinnable(true);
        re.setMinMaxValues(0, 255);
        wnd.label("Green: ");
        auto& ge = wnd.intbox<int>(IsEditable{ true });
        ge.setSpinnable(true);
        ge.setMinMaxValues(0, 255);
        wnd.label("Blue: ");
        auto& be = wnd.intbox<int>(IsEditable{ true });
        be.setSpinnable(true);
        be.setMinMaxValues(0, 255);
        wnd.label("Alpha: ");
        auto& ae = wnd.intbox<int>(IsEditable{ true });
        ae.setSpinnable(true);
        ae.setMinMaxValues(0, 255);

        wnd.button(Caption{ "Cancle" },
            FixedSize{ 60, 30 },
            ButtonCallback{ [&] {
              wnd.dispose();
            } });
        screen->performLayout();
        wnd.button(Caption{ "Create" },
            FixedSize{ 60, 30 },
            ButtonCallback{ [&, screen] {
                string item = screen->mCanvas->addLight(name.value(), type.selectedIndex(), vec3(positionX.value(), positionY.value(), positionZ.value()), vec3(re.value() / 255.f, ge.value() / 255.f, be.value() / 255.f), vec3(0.5f));
                Listbox* light_list = screen->findWidget<Listbox>("#light_list");
                light_list->addItem(item, item);
                screen->performLayout();
              wnd.dispose();
            } });
    }
    else {
        light->setVisible(show);
    }
}

void toggleMainMenu(ExampleApplication* screen, bool show)
{
    using namespace nanogui;

    auto menus = screen->findAll<WindowMenu>();

    if (menus.empty())
    {
        auto& mmenu = screen->wdg<WindowMenu>();
        mmenu.activate({ 0, 0 });
        auto dlg = [screen](std::string title) { screen->msgdialog(DialogTitle{ title }, DialogMessage{ "New Clicked" }); };
        mmenu.submenu("File")
            .item("New", [dlg]() { dlg("New"); })
            .item("Open", [dlg]() { dlg("Open"); })
            .item("Save", [dlg]() { dlg("Save"); });
        mmenu.submenu("Edit")
            .item("Undo", "Ctrl+Z", [dlg]() { dlg("Undo"); })
            .item("Redo", "Ctrl+Y", [dlg]() { dlg("Redo"); })
            .item("Cut", "Ctrl+X", [dlg]() { dlg("Cut"); })
            .item("Copy", "Ctrl+C", [dlg]() { dlg("Copy"); })
            .item("Paste", "Ctrl+V", [dlg]() { dlg("Paste"); });
        mmenu.submenu("Add")
            .item("Light", "", [screen]() {toggleAddLightWnd(screen, true); })
            .item("Model", "", [screen]() {toggleAddModelWnd(screen, true); });

        screen->performLayout();
    }
    else
    {
        menus.front()->setVisible(show);
    }
}

void toggleConsoleWnd(nanogui::Screen* screen, bool show)
{
    using namespace nanogui;

    auto console = screen->findWidgetGlobal("#console_wnd");
    if (!console)
    {
        auto& wnd = screen->window("Example: Console");
        wnd.setPosition(60, 60);
        wnd.withLayout<GroupLayout>();
        wnd.setFixedSize({ 300, 300 });
        wnd.setId("#console_wnd");
        screen->performLayout();
    }
    else
    {
        console->setVisible(show);
    }
}

void toggleLogWnd(nanogui::Screen* screen, bool show)
{
    using namespace nanogui;

    auto logwnd = screen->findWidgetGlobal("#log_wnd");
    if (!logwnd)
    {
        auto& wnd = screen->window("Example: Log");
        wnd.setPosition(120, 120);
        wnd.setFixedSize({ 400, 300 });
        wnd.setId("#log_wnd");
        screen->performLayout();
    }
    else
    {
        logwnd->setVisible(show);
    }
}

void toggleSimpleLayoutWnd(nanogui::Screen* screen, bool show)
{
    using namespace nanogui;

    auto logwnd = screen->findWidgetGlobal("#simple_layout_wnd");
    if (!logwnd)
    {
        auto& wnd = screen->window(Caption{ "Example: Simple layout" },
            WidgetStretchLayout{ Orientation::Horizontal },
            Position{ 180, 180 },
            MinimumSize{ 400, 400 },
            WidgetId{ "#simple_layout_wnd" });

        wnd.listbox(RelativeSize{ 0.33, 0 },
            ListboxCallback{ [screen](ListboxItem* i) {
              if (Label* lb = screen->findWidget<Label>("#simple_layout_lb"))
                lb->setCaption("MyObject: " + i->caption());
            } },
            ListboxContent{ [](Listbox& l) {
              for (int i = 0; i < 100; i++)
                l.addItem("Item " + std::to_string(i));
            } });
        auto& desc = wnd.widget(WidgetStretchLayout{ Orientation::Vertical });
        desc.label(Caption{ "MyObject: id" },
            CaptionHAlign{ TextHAlign::hLeft },
            FixedHeight{ 15 },
            WidgetId{ "#simple_layout_lb" });
        desc.tabs(TabNames{ "Description", "Details" });
        screen->performLayout();
    }
    else
    {
        logwnd->setVisible(show);
    }
}

void toggleTreeView(nanogui::Screen* screen, bool show)
{
    using namespace nanogui;

    auto treeview = screen->findWidgetGlobal("#tree_view_wnd");
    if (!treeview)
    {
        auto& wnd = screen->window(Caption{ "Example: Tree view" },
            WidgetStretchLayout{ Orientation::Horizontal },
            Position{ 180, 180 },
            MinimumSize{ 400, 400 },
            WidgetId{ "#tree_view_wnd" });

        auto& view = wnd.treeview(RelativeSize{ 0.5, 0 }, WidgetId{ "#treeview" });
        auto n1 = view.rootNode()->addNode("Node1");
        auto n1_c1 = n1->addNode("Node1_C1");
        n1_c1->addNode("NodeC1_c1");
        auto n2 = view.rootNode()->addNode("Node2");
        n2->addNode("Some text here");
        view.rootNode()->addNode("Node3");
        view.rootNode()->addNode("Node4");
        auto n5 = view.rootNode()->addNode("Node5");
        n5->addNode("Node5_C1")
            ->addNode("yet one node")
            ->addNode("yet second node")
            ->addNode("and third node")
            ->addNode("anybody show me?");
    }
    else
    {
        treeview->setVisible(show);
    }
}

ExampleApplication::ExampleApplication() : nanogui::Screen(nanogui::Vector2i(1400, 700), "Sora PathTracer", true) {
    using namespace nanogui;

    config = new Config(1, DIFFUSE, 1, 0.2, SAH, OPENGL);
    camera = new Camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.f, 0.f, 0.f), 1.0f);

    //tabHeader = new TabHeader(this);
    //tabHeader->addTab("Add");
    //tabHeader->addTab("label2");
    //tabHeader->addTab("label3");
    //tabHeader->setActiveTab(-1);
    //tabHeader->setCallback([this](int i) { 

    //    std::cout << i << std::endl; 
    //    });

    toggleMainMenu(this, true);
    //createAllWidgetsDemo(this);

    FormHelper* form = new FormHelper(this);
    ref<Window> setting = form->addWindow(Vector2i(MARGIN, 3 * MARGIN), "Settings");
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

    Window* renderWindow = new Window(this, "Render Result");
    renderWindow->setPosition(Vector2i(FORM_WIDTH + 2 * MARGIN, 3 * MARGIN));
    renderWindow->setLayout(new GroupLayout());
    renderWindow->setFixedWidth(600);
    renderWindow->setFixedHeight(600);

    mCanvas = new MyGLCanvas(renderWindow, camera);
    mCanvas->setBackgroundColor({ 100, 100, 100, 255 });
    mCanvas->setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

    FormHelper* form1 = new FormHelper(this);
    ref<Window> setting1 = form1->addWindow(Vector2i(FORM_WIDTH + 5 * MARGIN + SCREEN_WIDTH, 3 * MARGIN), "Settings1");
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

    //auto& properties =this->wdg<PropertiesEditor>(Caption{ "Properties" },
    //    WidgetId{ "#prop_editor" },
    //    Position{ this->width() - 370, 30 },
    //    FixedSize{ 300, this->height() - 400 },
    //    WindowMovable{ Theme::dgFixed });
    //auto* propertyLayout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 2, 5);
    //propertyLayout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
    //properties.setLayout(propertyLayout);

    auto& modelList = this->window(Caption{ "Model Settings" },
        WidgetStretchLayout{ Orientation::Horizontal },
        Position{ 180, 180 },
        MinimumSize{ 400, 400 },
        WidgetId{ "#model_layout_wnd" });

    modelList.listbox(RelativeSize{ 0.33, 0 },
        ListboxCallback{ [this](ListboxItem* i) {
          if (Label* lb = this->findWidget<Label>("#model_layout_lb"))
            lb->setCaption("Model: " + i->caption());
        } },
        WidgetId{"#model_list"}
        );
    auto& modelDesc = modelList.widget(WidgetStretchLayout{ Orientation::Vertical });
    modelDesc.label(Caption{ "Model: id" },
        CaptionHAlign{ TextHAlign::hLeft },
        FixedHeight{ 40 },
        WidgetId{ "#model_layout_lb" });
    modelDesc.tabs(TabNames{ "Location", "Material" });

    auto& lightList = this->window(Caption{ "Light Settings" },
        WidgetStretchLayout{ Orientation::Horizontal },
        Position{ 480, 180 },
        MinimumSize{ 400, 400 },
        WidgetId{ "#light_layout_wnd" });
    //static_cast<FloatBox<float>*>(this->findWidget("#light_position.x"))    
    selectedLight = nullptr;

    auto& lightDesc = lightList.widget(WidgetStretchLayout{ Orientation::Vertical });
    lightDesc.label(Caption{ "Light: id" },
        CaptionHAlign{ TextHAlign::hLeft },
        FixedHeight{ 40 },
        WidgetId{ "#light_layout_lb" });
    //lightDesc.tabs(TabNames{ "Information", "Material" });
    auto& tabWidget = lightDesc.tabs();

    auto& lightInformation = *tabWidget.createTab("Information");
    auto& vscroll = lightInformation.vscrollpanel();
    auto& wrapper = vscroll.widget();
    wrapper.setLayout(new GroupLayout(5, 5, 5, 10));
    vscroll.setFixedSize({ 200, 900 });
    wrapper.label(Caption{ "Position" }, CaptionFont{ "sans-bold" });
    auto& positionX = wrapper.widget();
    positionX.flexlayout(Orientation::Horizontal);
    positionX.label(Caption{ "X" },
        CaptionFont{ "sans" },
        RelativeSize{ 0.5, 0 });
    auto& positionXValue = positionX.wdg<FloatBox<float>>();
    positionXValue.setValue(0);
    positionXValue.setSpinnable(true);
    positionXValue.setEditable(true);
    positionXValue.setId("#light_position_x");
    positionXValue.setCallback([this](float x) {
        if (selectedLight == nullptr)
            return;
        selectedLight->position.x = x;
        });

    auto& positionY = wrapper.widget();
    positionY.flexlayout(Orientation::Horizontal);
    positionY.label(Caption{ "Y" },
        CaptionFont{ "sans" },
        RelativeSize{ 0.5, 0 });
    auto& positionYValue = positionY.wdg<FloatBox<float>>(1.f);
    positionYValue.setSpinnable(true);
    positionYValue.setEditable(true);
    positionYValue.setId("#light_position_y");
    positionYValue.setCallback([this](float y) {
        if (selectedLight == nullptr)
            return;
        selectedLight->position.y = y;
        });

    auto& positionZ = wrapper.widget();
    positionZ.flexlayout(Orientation::Horizontal);
    positionZ.label(Caption{ "Z" },
        CaptionFont{ "sans" },
        RelativeSize{ 0.5, 0 });
    auto& positionZValue = positionZ.wdg<FloatBox<float>>(1.f);
    positionZValue.setSpinnable(true);
    positionZValue.setEditable(true);
    positionZValue.setId("#lgiht_position_z");
    positionZValue.setCallback([this](float z) {
        if (selectedLight == nullptr)
            return;
        selectedLight->position.z = z;
        });

    wrapper.label(Caption{ "Color" }, CaptionFont{ "sans-bold" });
    auto& lightR = wrapper.widget();
    lightR.flexlayout(Orientation::Horizontal);
    lightR.label(Caption{ "R" },
        CaptionFont{ "sans" },
        RelativeSize{ 0.5, 0 });
    auto& lightRValue = lightR.wdg<IntBox<int>>(1);
    lightRValue.setSpinnable(true);
    lightRValue.setEditable(true);


    auto& lightMaterial = *tabWidget.createTab("Material");
    tabWidget.setActiveTab(0);

    lightList.listbox(RelativeSize{ 0.33, 0 },
        ListboxCallback{ [&](ListboxItem* i) {
            selectedLight = this->mCanvas->findLight(i->caption());
            positionXValue.setValue(selectedLight->position.x);
            positionYValue.setValue(selectedLight->position.y);
            positionZValue.setValue(selectedLight->position.z);
          if (Label* lb = this->findWidget<Label>("#light_layout_lb"))
            lb->setCaption("Light: " + i->caption());
        } },
        WidgetId{ "#light_list" }
        );
    
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
        std::cout << "debug1" << std::endl;
        setVisible(false);
        return true;
    }
    return false;
    //if (Screen::keyboardEvent(key, scancode, action, modifiers))
    //    return true;
    //if (nanogui::isKeyboardKeyEscape(key) && nanogui::isKeyboardActionPress(action)) {
    //    setVisible(false);
    //    return true;
    //}
}

//bool ExampleApplication::keyCallbackEvent(int key, int scancode, int action, int mods) {
//    std::cout << "he " << key << std::endl;
//    //if (Screen::keyboardEvent(key, scancode, action, modifiers)) {
//    //    return true;
//    //}
//    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//    //    setVisible(false);
//    //    return true;
//    //}
//    if (Screen::keyCallbackEvent(key, scancode, action, mods))
//        return true;
//    if (nanogui::isKeyboardKeyEscape(key) && nanogui::isKeyboardActionPress(action)) {
//        setVisible(false);
//        return true;
//    }
//    return false;
//}

void ExampleApplication::draw(NVGcontext* ctx) {
    /* Draw the user interface */
    Screen::draw(ctx);
}