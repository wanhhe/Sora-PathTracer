#ifndef PATHTRACER_RENDERER_H
#define PATHTRACER_RENDERER_H

#include "common.h"

class Renderer {
public:
	Renderer() = default;
	void clear();
	void render();

	void setModel(const mat4& _model);
	void setView(const mat4& _view);
	void setPorjection(const mat4& _projection);
private:
	mat4 model;
	mat4 view;
	mat4 projection;
};
#endif // !PATHTRACER_RENDERER_H
