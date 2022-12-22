#ifndef PATHTRACER_APPLICATION_H
#define PATHTRACER_APPLICATION_H

#include <nanogui/nanogui.h>
#include <nanogui/widget.h>

#include "common.h"

class Application: public nanogui::Screen {
    public:
        Application();
        void run();
        void showInit();
        void showFrameBuffer(uint8_t *data, int width, int height);
        void drawAll();

    private:
        Config *config;
        nanogui::Button *start;
        nanogui::ProgressBar *progressBar;
        nanogui::Window *image;
        nanogui::ImageView *imageView;
        float imageDataProgress = 0.0;
        bool imageDataSignal = false;

        void readModel();
        void loadModel();
};

#endif // PATHTRACER_APPLICATION_H