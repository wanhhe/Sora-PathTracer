#ifndef PATHTRACER_RENDERER_H
#define PATHTRACER_RENDERER_H

#include <thread>

#include "common.h"
#include "scene.h"
#include "camera.h"

class Renderer {
public:
	Renderer() = default;
	void init(Config* config);
	void clear();
	void render();

	void setModel(const mat4& _model);
	void setView(const mat4& _view);
	void setPorjection(const mat4& _projection);
private:
	mat4 model;
	mat4 view;
	mat4 projection;

	int currentSampleCount = 0;

	Config* config;
	Scene* scene;
	Camera* camera;

	vector<std::thread> threaPool;
	vector<vector<vec3>> sampleFramebufferPool;
	vector<vec3> framebuffer;
	
};
#endif // !PATHTRACER_RENDERER_H
