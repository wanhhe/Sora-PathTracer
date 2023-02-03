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
	nanogui::GLShader hairShadowShader;
	nanogui::GLShader testhairShader;
	nanogui::GLShader testotherShader;
	nanogui::GLShader lastFaceShader;
	nanogui::GLShader hairShadowStencilShader;
	unsigned int bodyLightMap;
	unsigned int hairLightMap;
	unsigned int hairSpecularMap;
	unsigned int sdfMap;
	unsigned int fbo;
	unsigned int captureRBO;
	unsigned int tbo;
	vec3 lightPos;
	vec3 lightColor;
	vec3 ambientColor;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	unsigned int loadTexture(char const* path);
	void renderQuad();
};

#endif // !PATHTRANCER_NPR_SHADER_H
