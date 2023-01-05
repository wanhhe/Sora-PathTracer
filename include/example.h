#include "canvas.h"
#include <nanovg.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/dial.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/meter.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/table.h>
#include <nanogui/graph.h>
#include <nanogui/ledmatrix.h>
#include <nanogui/tabwidget.h>
#include <nanogui/switchbox.h>
#include <nanogui/spinner.h>
#include <nanogui/dropdownbox.h>
#include <nanogui/editworkspace.h>
#include <nanogui/editproperties.h>
#include <nanogui/scrollbar.h>
#include <nanogui/windowmenu.h>
#include <nanogui/perfchart.h>
#include <nanogui/common.h>
#include <nanogui/listbox.h>
#include <nanogui/themebuilder.h>
#include <nanogui/tolerancebar.h>
#include <nanogui/treeview.h>
#include <nanogui/treeviewitem.h>
#include <nanogui/picflow.h>
#include <nanogui/textarea.h>
#include <nanogui/searchbox.h>
#include <nanogui/splitter.h>
#include <nanogui/editproperties.h>
#include <nanogui/formhelper.h>
#include <iostream>
#include <sstream>
#include <string>
#include <numeric>
#include <tuple>

class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication();

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

    //bool keyCallbackEvent(int key, int scancode, int action, int mods);

    virtual void draw(NVGcontext* ctx) override;

    MyGLCanvas* mCanvas;

private:
    
    //nanogui::TabHeader* tabHeader;
    Config* config;
    Camera* camera;
};