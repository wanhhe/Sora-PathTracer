#include "canvas.h"

class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication();

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

    virtual void draw(NVGcontext* ctx) override;

private:
    MyGLCanvas* mCanvas;
};