#include "shader.h"

bool LightShader::init(const std::string& name, const std::string& vertex_str,
    const std::string& fragment_str,
    const std::string& geometry_str = "") {
    return init(name, vertex_str,
        "#version 330 core\n"
        "out vec4 FragColor\n"
        "void main() {\n"
        "   FragColor = vec4(1.0);\n"
        "}\n"
    , geometry_str);
}

bool DiffuseShader::init(const std::string& name, const std::string& vertex_str,
    const std::string& fragment_str,
    const std::string& geometry_str = "") {
    return init(name, vertex_str, fragment_str, geometry_str);
}

bool SpecularShader::init(const std::string& name, const std::string& vertex_str,
    const std::string& fragment_str,
    const std::string& geometry_str = "") {
    return init(name, vertex_str, fragment_str, geometry_str);
}

bool BlinnPhongShader::init(const std::string& name, const std::string& vertex_str,
    const std::string& fragment_str,
    const std::string& geometry_str = "") {
    return init(name, vertex_str, fragment_str, geometry_str);
}

bool MicrofacetShader::init(const std::string& name, const std::string& vertex_str,
    const std::string& fragment_str,
    const std::string& geometry_str = "") {
    return init(name, vertex_str, fragment_str, geometry_str);
}