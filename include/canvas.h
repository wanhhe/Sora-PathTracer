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

    // �ƶ��������������
    virtual bool scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) override;

    /// �϶�����Ҽ�ƽ��������϶���������ת���
    virtual bool mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;

private:
    nanogui::GLShader mShader;
    mat4 model;
    mat4 view;
    mat4 projection;
    Camera* camera;
};