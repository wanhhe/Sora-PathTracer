#include <nanogui/nanogui.h>
#include "application.h"

int main() {
    try
    {
        nanogui::init();

        {
            Application *application = new Application();
            application->drawAll();
            application->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    }
    catch(const std::runtime_error& e)
    {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << '\n';
        return -1;
    }
    
}