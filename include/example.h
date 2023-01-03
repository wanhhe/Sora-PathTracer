#include "canvas.h"

class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication();

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

    //bool keyCallbackEvent(int key, int scancode, int action, int mods);

    virtual void draw(NVGcontext* ctx) override;

private:
    MyGLCanvas* mCanvas;
    Config* config;
    Camera* camera;
};