#include <nanogui/nanogui.h>

#include "camera.h"

class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget* parent);

    ~MyGLCanvas() {
        mShader.free();
    }

    virtual void drawGL();

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

private:
    nanogui::GLShader mShader;
    mat4 model;
    mat4 view;
    mat4 projection;
    Camera* camera;
};