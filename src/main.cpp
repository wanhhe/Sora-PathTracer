//#include <nanogui/nanogui.h>
//#include <nanogui/common.h>
//#include "application.h"

//int main() {
//    try
//    {
//        nanogui::init();
//
//        {
//            Application *application = new Application();
//            application->drawAll();
//            application->setVisible(true);
//            nanogui::mainloop();
//        }
//
//        nanogui::shutdown();
//    }
//    catch(const std::runtime_error& e)
//    {
//        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
//        std::cerr << error_msg << '\n';
//        return -1;
//    }
//    
//}


/*
    src/example4.cpp -- C++ version of an example application that shows
    how to use the OpenGL widget. For a Python implementation, see
    '../python/example4.py'.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

//#include <nanogui/window.h>
//#include <nanogui/layout.h>
//#include <nanogui/entypo.h>
//#include <nanogui/vscrollpanel.h>
//#include <nanogui/colorwheel.h>
//#include <nanogui/graph.h>
//#include <nanogui/glcanvas.h>
//#include <iostream>
//#include <string>
////
////// Includes for the GLTexture class.
//#include <cstdint>
//#include <memory>
//#include <utility>
//
//#include "example.h"
//
//#if defined(__GNUC__)
//#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
//#endif
//#if defined(_WIN32)
//#  pragma warning(push)
//#  pragma warning(disable: 4457 4456 4005 4312)
//#endif
//
//#if defined(_WIN32)
//#  pragma warning(pop)
//#endif
//#if defined(_WIN32)
//#  if defined(APIENTRY)
//#    undef APIENTRY
//#  endif
//#  include <windows.h>
//#endif
//
//using std::cout;
//using std::cerr;
//using std::endl;
//using std::string;
//using std::vector;
//using std::pair;
//using std::to_string;
//
//int main(int /* argc */, char** /* argv */) {
//    try {
//        nanogui::init();
//
//        /* scoped variables */ {
//            nanogui::ref<ExampleApplication> app = new ExampleApplication();
//            app->drawAll();
//            app->setVisible(true);
//            nanogui::mainloop();
//        }
//
//        nanogui::shutdown();
//    }
//    catch (const std::runtime_error& e) {
//        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
//#if defined(_WIN32)
//        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
//#else
//        std::cerr << error_msg << endl;
//#endif
//        return -1;
//    }
//
//    return 0;
//}

#include <nanogui/window.h>

#include <iostream>
#include <string>

#include "example.h"

int main(int /* argc */, char** /* argv */) {
    nanogui::init();

    nanogui::Vector2i size(1400, 700);
    auto window = nanogui::sample::create_window(size.x(), size.y(), "NanoGUI test", true, false, true);
    nanogui::sample::create_context();

    /* scoped variables */ {
        ExampleApplication screen;
        nanogui::sample::setup_window_params(window, &screen);

        screen.drawAll();
        screen.setVisible(true);

        nanogui::sample::run([&] {
            nanogui::sample::clear_frame(screen.background());

            screen.drawAll();

            nanogui::sample::present_frame(window);

            /* Wait for mouse/keyboard or empty refresh events */
            nanogui::sample::wait_events();
            });

        nanogui::sample::poll_events();
    }

    nanogui::sample::destroy_window(window);
    nanogui::shutdown();
    return 0;
}