#include "example.h"

ExampleApplication::ExampleApplication() : nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", false) {
    using namespace nanogui;

    Window* window = new Window(this, "GLCanvas Demo");
    window->setPosition(Vector2i(15, 15));
    window->setLayout(new GroupLayout());

    mCanvas = new MyGLCanvas(window);
    mCanvas->setBackgroundColor({ 100, 100, 100, 255 });
    mCanvas->setSize({ 400, 400 });

    Widget* tools = new Widget(window);
    tools->setLayout(new BoxLayout(Orientation::Horizontal,
        Alignment::Middle, 0, 5));

    Button* b0 = new Button(tools, "Random Color");
    b0->setCallback([this]() { mCanvas->setBackgroundColor(Vector4i(rand() % 256, rand() % 256, rand() % 256, 255)); });

    performLayout();
}

bool ExampleApplication::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
        return true;
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