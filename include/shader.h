#ifndef PATHTRACER_SHADER_H
#define PATHTRACER_SHADER_H

#include <nanogui/glutil.h>

class LightShader: public nanogui::GLShader {
    bool init(const std::string& name, const std::string& vertex_str,
        const std::string& fragment_str,
        const std::string& geometry_str = "");
};

class DiffuseShader: public nanogui::GLShader {
    bool init(const std::string& name, const std::string& vertex_str,
        const std::string& fragment_str,
        const std::string& geometry_str = "");
};

class SpecularShader: public nanogui::GLShader {
    bool init(const std::string& name, const std::string& vertex_str,
        const std::string& fragment_str,
        const std::string& geometry_str = "");
};

class BlinnPhongShader: public nanogui::GLShader {
    bool init(const std::string& name, const std::string& vertex_str,
        const std::string& fragment_str,
        const std::string& geometry_str = "");
};

class MicrofacetShader: public nanogui::GLShader {
    bool init(const std::string& name, const std::string& vertex_str,
        const std::string& fragment_str,
        const std::string& geometry_str = "");
};

#endif // !PATHTRACER_SHADER_H
