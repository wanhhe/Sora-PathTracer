#ifndef PATHTRANCER_NPR_SHADER_H
#define PATHTRANCER_NPR_SHADER_H

#include "common.h"
#include "model.h"
#include <nanogui/glutil.h>

class NPRShader {
public:
	NPRShader();
	void draw(const mat4& view, const mat4& projection, const vec3& viewPos);
private:
	Model* model;
	nanogui::GLShader shader;
	nanogui::GLShader colorBufferShader;
	unsigned int bodyLightMap;
	unsigned int hairLightMap;
	unsigned int hairSpecularMap;
	unsigned int sdfMap;
	vec3 lightPos;
	vec3 lightColor;
	vec3 ambientColor;

	unsigned int loadTexture(char const* path);
};

#endif // !PATHTRANCER_NPR_SHADER_H
