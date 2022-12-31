#include <nanogui/nanogui.h>

#include "camera.h"
#include "model.h"

class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget* parent);

    ~MyGLCanvas() {
        mShader.free();
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

private:
    nanogui::GLShader mShader;
    mat4 model;
    mat4 view;
    mat4 projection;
    Camera* camera;
    Model hikari;
};