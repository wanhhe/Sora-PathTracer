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
                _screen->mCanvas->addModel(text.value(), name.value());
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
                //screen->mCanvas->addLight(name.value(), type.selectedIndex(), vec3(positionX.value(), positionY.value(), positionY.value()), vec3(re.value() / 255.f, ge.value() / 255.f, be.value() / 255.f), vec3(0.5f));
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

//void createAllWidgetsDemo(nanogui::Screen* screen) {
//    using namespace nanogui;
//    Window& dw = screen->window(WidgetStretchLayout{ Orientation::Vertical },
//        Caption{ "All widgets demo" },
//        Position{ 725, 350 },
//        FixedSize{ 400, 400 });
//
//    dw.submenu("File")
//        .item("(dummy item)", []() {})
//        .item("New", "Ctrl+N", [screen]() { screen->msgdialog(DialogTitle{ "New" }, DialogMessage{ "New Clicked!" }); })
//        .item("Very larget text", [screen]() { screen->msgdialog(DialogTitle{ "Open" }, DialogMessage{ "New Clicked!" }); })
//        .item("Save", [screen]() { screen->msgdialog(DialogTitle{ "Save" }, DialogMessage{ "New Clicked!" }); });
//    dw.submenu("File").item("(dummy item)").setEnabled(false);
//    dw.submenu("File").item("Save").setShortcut("Ctrl+S");
//
//    auto toggleVisible = [screen](const std::string& /*wname*/, bool& enabled, bool& checked) {
//        enabled = true;
//        auto* w = screen->findWidgetGlobal("#console_wnd");
//        checked = (w && w->visible());
//    };
//    dw.submenu("Examples")
//        .item("Global menu", [screen](bool v) { toggleMainMenu(screen, v); })
//        .item("Console", [screen](bool v) { toggleConsoleWnd(screen, v); }, [=](bool& e, bool& c) { toggleVisible("#console_wnd", e, c); })
//        .item("Log", [screen](bool v) { toggleLogWnd(screen, v); }, [=](bool& e, bool& c) { toggleVisible("#log_wnd", e, c); })
//        .item("Simple layout", [screen](bool v) { toggleSimpleLayoutWnd(screen, v); }, [=](bool& e, bool& c) { toggleVisible("#simple_layout_wnd", e, c); })
//        .item("Tree view", [screen](bool v) { toggleTreeView(screen, v); }, [=](bool& e, bool& c) { toggleVisible("#tree_view_wnd", e, c); });
//
//    dw.submenu("Help");
//
//    auto& pw = dw.vscrollpanel(RelativeSize{ 1.f, 1.f }).vstack(2, 2);
//
//    auto& iocfg = pw.panel(Caption{ "Configuration" }, WindowCollapsed{ true });
//    auto& nav = iocfg.panel(Caption{ "Configuration" }, WindowCollapsed{ true }, PanelHighlightHeader{ false });
//    auto gs = [screen] { return screen->theme(); };
//    nav.checkbox(Caption{ "theme.nav.mouse.enable" }, BoolObservable{ [gs] {return gs()->nav.mouse.enable; },
//                                                                      [gs](bool v) { gs()->nav.mouse.enable = v; } });
//    nav.checkbox(Caption{ "theme.nav.mouse.drawCursor" }, BoolObservable{ [gs] {return gs()->nav.mouse.drawCursor; },
//                                                                          [gs](bool v) { gs()->nav.mouse.drawCursor = v; } });
//    nav.checkbox(Caption{ "theme.textAreaBlinkCursor" }, BoolObservable{ [gs] {return gs()->textAreaBlinkCursor; },
//                                                                         [gs](bool v) { gs()->textAreaBlinkCursor = v; } });
//    nav.checkbox(Caption{ "theme.windowMoveFromTitlebarOnly" }, BoolObservable{ [gs] {return gs()->windowMoveFromTitlebarOnly; },
//                                                                                [gs](bool v) { gs()->windowMoveFromTitlebarOnly = v; } });
//    nav.checkbox(Caption{ "theme.windowEesizeFromEdge" }, BoolObservable{ [gs] {return gs()->windowResizeFromEdge; },
//                                                                          [gs](bool v) { gs()->windowResizeFromEdge = v; } });
//    nav.checkbox(Caption{ "theme.windowMoveInParent" }, BoolObservable{ [gs] {return gs()->windowMoveInParent; },
//                                                                        [gs](bool v) { gs()->windowMoveInParent = v; } });
//    nav.checkbox(Caption{ "theme.windowDrawBorder" }, BoolObservable{ gs()->windowDrawBorder });
//    nav.checkbox(Caption{ "theme.frameDrawBorder" }, BoolObservable{ [gs] {return gs()->frameDrawBorder; },
//                                                                     [gs](bool v) { gs()->frameDrawBorder = v; } });
//    nav.checkbox(Caption{ "theme.debugHighlightMouseover" }, BoolObservable{ [gs] {return gs()->debugHighlightMouseover; },
//                                                                             [gs](bool v) { gs()->debugHighlightMouseover = v; } });
//    nav.checkbox(Caption{ "theme.keyboardNavigation" }, BoolObservable{ [gs] {return gs()->keyboardNavigation; },
//                                                                        [gs](bool v) { gs()->keyboardNavigation = v; } });
//
//    /*auto& bfcfg = */iocfg.panel(Caption{ "Backend flags" }, WindowCollapsed{ true }, PanelHighlightHeader{ false });
//    auto& stcfg = iocfg.hgrid2(0.3f, Caption{ "Style" }, WindowCollapsed{ true }, PanelHighlightHeader{ false });
//    stcfg.label("Theme");
//    stcfg.wdg<DropdownBox>(DropdownBoxItems{ "Default", "White" },
//        DropdownBoxStrCallback{ [screen](std::string item) {
//          if (item == "Default") screen->setTheme<DefaultTheme>();
//          else if (item == "White") screen->setTheme<WhiteTheme>();
//        } });
//    auto screenPerform = SliderCallback{ [screen](float) { screen->needPerformLayout(screen); } };
//    stcfg.add(elm::Label{ "Window border size" }, Element<Slider>{ SliderObservable{ gs()->windowPaddingLeft }, SliderRange{ 0.f, 20.f }, screenPerform});
//    stcfg.add(elm::Label{ "Window border size" }, Element<Slider>{ SliderObservable{ gs()->windowBorderSize }, SliderRange{ 0.f, 5.f }, screenPerform });
//    stcfg.add(elm::Label{ "Window padding top" }, Element<Slider>{ SliderObservable{ gs()->windowPaddingTop }, SliderRange{ 0.f, 20.f }, screenPerform });
//    stcfg.add(elm::Label{ "Frame padding left" }, Element<Slider>{ SliderObservable{ gs()->framePaddingLeft }, SliderRange{ 0.f, 20.f }, screenPerform });
//    stcfg.add(elm::Label{ "Frame padding top" }, Element<Slider>{ SliderObservable{ gs()->framePaddingTop }, SliderRange{ 0.f, 20.f }, screenPerform });
//    stcfg.add(elm::Label{ "Inner spacing left" }, Element<Slider>{ SliderObservable{ gs()->innerSpacingCommon }, SliderRange{ 0.f, 20.f }, screenPerform});
//    stcfg.add(elm::Label{ "Tool button side" },
//        Element<Slider>{
//        SliderObservable{ [=] { return (float)gs()->toolButtonSide; }, [=](float v) { gs()->toolButtonSide = (int)v; } },
//            SliderRange{ 15.f, 50.f }, screenPerform
//    });
//
//    auto& wopt = iocfg.hgrid2(0.5f, Caption{ "Window options" }, WindowCollapsed{ true });
//    auto dwf = [screen, w = &dw](int f, int v = -1) {
//        if (v < 0) return w->haveDrawFlag(f);
//        w->setDrawFlag(f, v > 0);
//        screen->needPerformLayout(screen);
//        return false;
//    };
//    wopt.checkbox(Caption{ "No header" }, BoolObservable{ [=] {return !dwf(Window::DrawHeader); },
//                                                          [=](bool v) { dwf(Window::DrawHeader, !v); } });
//    wopt.checkbox(Caption{ "No title" }, BoolObservable{ [=] {return !dwf(Window::DrawTitle); },
//                                                          [=](bool v) { dwf(Window::DrawTitle, !v); } });
//    wopt.checkbox(Caption{ "No collapse icon" }, BoolObservable{ [=] {return !dwf(Window::DrawCollapseIcon); },
//                                                                 [=](bool v) { dwf(Window::DrawCollapseIcon, !v); } });
//    wopt.checkbox(Caption{ "No move" }, BoolObservable{ [w = &dw] { return !w->isDraggable(); }, [w = &dw](bool v) { w->setDraggable((Theme::WindowDraggable)!v); } });
//    wopt.checkbox(Caption{ "No resize" }, BoolObservable{ [w = &dw] { return !w->canResize(); }, [w = &dw](bool v) { w->setCanResize(!v); } });
//    wopt.checkbox(Caption{ "No background" }, BoolObservable{ [=] {return !dwf(Window::DrawBody); }, [=](bool v) { dwf(Window::DrawBody, !v); } });
//    wopt.checkbox(Caption{ "No bring to front" }, BoolObservable{ [w = &dw] {return !w->canBringToFront(); }, [w = &dw](bool v) {w->setBringToFront(!v); } });
//
//    auto& wwidgets = pw.panel(Caption{ "Widgets" }, WindowCollapsed{ true });
//    auto& wbasic = wwidgets.panel(Caption{ "Basic" }, WindowCollapsed{ true }, PanelHighlightHeader{ false });
//    wbasic.hstack(5, 2,
//        elm::Button{
//              Caption{"Button"}, FixedHeight{17},
//              ButtonCallback{ [w = &dw] { if (auto l = Label::find("#btn_action", w)) l->setCaption("Thanks for cliking me!!!"); }}
//        },
//        elm::Label{ WidgetId{ "#btn_action" } });
//    wbasic.checkbox(Caption{ "checkbox" });
//    auto radio_action = ButtonChangeCallback{ [w = &dw](Button* b) {
//      if (b && !b->pushed())
//        return;
//      if (auto l = Label::find("#radio_action", w))
//        l->setCaption("Clicked " + b->caption());
//    } };
//    auto makerbtn = [=](std::string text) { return elm::RadioBtn{ Caption{text}, radio_action, FixedHeight{17} }; };
//    wbasic.hstack(5, 2,
//        makerbtn("radio a"), makerbtn("radio b"), makerbtn("radio c"),
//        elm::Label{ WidgetId{ "#radio_action" } });
//
//    auto makecbtn = [=](Color c) { return elm::Button{ Caption{ "Click" }, FixedHeight{ 17 }, BackgroundColor{c} }; };
//    wbasic.hstack(5, 2, makecbtn(Color::red), makecbtn(Color::yellow), makecbtn(Color::green),
//        makecbtn(Color::blue), makecbtn(Color::purple), makecbtn(Color::pink));
//
//    wbasic.hstack(5, 2,
//        elm::Label{ "Hold to repeat:" },
//        elm::UpDownButton{ UpDownCallback{ [w = &dw](bool v) {
//            static int value = 0;
//            value += (v ? 1 : -1);
//            if (auto l = Label::find("#updown_action", w))
//              l->setCaption(std::to_string(value));
//          }
//        } },
//        elm::Label{ WidgetId{ "#updown_action"} });
//
//    wbasic.hstack(5, 2,
//        elm::Label{ Caption{"Hover over me"}, TooltipText{"I am tooltip"} },
//        elm::Label{ " - " },
//        elm::Label{ Caption{"or me"},
//                   TooltipWidget<Window>{
//                      IsVisible{ false },
//                      WidgetStretchLayout{ Orientation::Vertical },
//                      Caption{ "And I am tooltip too" },
//                      FixedSize{ 100, 60 },
//                      makecbtn(Color::red), makecbtn(Color::yellow), makecbtn(Color::green)
//                   }
//        });
//    wbasic.widget(FixedHeight{ 2 }, elm::SplitLine{});
//    wbasic.frame(WidgetGridLayout{ GridLayoutSplit{ 0.7f, 0.3f }, GridLayoutColAlignment{ Alignment::Fill } },
//        elm::Label{ "Value" },
//        elm::Label{ "Caption" },
//        elm::DropdownBox{
//          DropdownBoxFill{ [](string& r) { static char i = 'a'; r = std::string(5, i); return i++ < 'z';  }},
//          ItemHeight{ 20 }
//        },
//        elm::Label{ Caption{"combo (?)"}, TooltipText{ "Combo section fill function example" } },
//        elm::Textbox{
//          TextValue{"Input text here!"}, IsEditable{true}, FixedHeight{20},
//          TextBoxEditCallback{ [](const std::string& s, bool) { if (auto l = Label::find("#inp_txt_smp")) l->setCaption(s); }}
//        },
//        elm::Label{ Caption{"Input text"}, WidgetId{"#inp_txt_smp"} },
//        elm::Textbox{ TextPlaceholder{"input text here"}, IsEditable{ true }, FixedHeight{ 20 } },
//        elm::Label{ Caption{ "Input text (w/ hint)" } }
//
//    );
//}

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

    auto& properties =this->wdg<PropertiesEditor>(Caption{ "Properties" },
        WidgetId{ "#prop_editor" },
        Position{ this->width() - 370, 30 },
        FixedSize{ 300, this->height() - 400 },
        WindowMovable{ Theme::dgFixed });
    auto* propertyLayout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 2, 5);
    propertyLayout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
    properties.setLayout(propertyLayout);
    properties.label("test");
    properties.intbox<int>(IsEditable{true});

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