#ifndef PATHTRACER_COMMON_H
#define PATHTRACER_COMMON_H

#include <iostream>
#include <vector>
#include <stack>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define PI 3.1415926535
#define TWO_PI 6.283185307
#define INV_PI 0.31830988618
#define INV_TWO_PI 0.15915494309
#define EPSILON 1e-5f

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define FORM_WIDTH 260
#define FORM_HEIGHT 380
#define VALUE_WIDTH 120
#define MARGIN 16
#define PAD_WIDTH 32
#define PAD_HEIGHT 54
#define APP_HEIGHT (SCREEN_HEIGHT + PAD_HEIGHT > FORM_HEIGHT + 100 ? SCREEN_HEIGHT + PAD_HEIGHT : FORM_HEIGHT + 100)

#define RUSSIAN_ROULETTE = 0.8

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::stack;
using std::shared_ptr;
using std::make_shared;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::radians;
using glm::tan;
using glm::sin;
//using glm::cos;
using glm::normalize;
using glm::dot;
using glm::cross;
using glm::lookAt;
using glm::linearRand;


enum MaterialEnum {
    DIFFUSE = 0,
    MIRROR,
    MICROFACET
};

enum AccelStructureEnum {
    ACCEL_NONE = 0,
    BVH,
    SAH
};

enum RenderModeEnum {
    NAIVE = 0,
    OPENGL
};

typedef struct Config {
    int sampleCount;
    MaterialEnum material;
    float roughness;
    int threadCount;
    AccelStructureEnum accelStructure;
    RenderModeEnum renderMode;

    Config(int _sampleCount, MaterialEnum _material, int _threadCount, float _roughness, AccelStructureEnum _accelStructure, RenderModeEnum _renderMode): 
        sampleCount(_sampleCount), material(_material), threadCount(_threadCount), roughness(_roughness), accelStructure(_accelStructure), renderMode(_renderMode) {}
} Config;

#endif // PATHTRACER_COMMON_H