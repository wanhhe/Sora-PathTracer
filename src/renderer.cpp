#include "renderer.h"

void Renderer::clear() {
	scene = nullptr;
	framebuffer.clear();
	sampleFramebufferPool.clear();
	currentSampleCount = 0;
}

void Renderer::render() {
	int blockLength = floor(1.0f * SCREEN_HEIGHT / config->threadCount);

	threaPool.clear();
	for (int threadIndex = 0; threadIndex < config->threadCount; threadIndex++) {
		std::thread t([this, threadIndex, blockLength]() {
			int begin = threadIndex * blockLength;
			int end = (threadIndex == config->threadCount) ? SCREEN_HEIGHT : (threadIndex + 1) * blockLength;
			for (int i = begin; i < end; i++) {
				for (int j = 0; j < SCREEN_WIDTH; j++) {
					Ray ray = camera->getRayRandom(i, j);
					vec3 pixel = scene->castRay(ray);
					sampleFramebufferPool[threadIndex].emplace_back(pixel);
				}
			}
			});
		threaPool.emplace_back(std::move(t));
	}
	for (auto& t : threaPool) t.join();

	int pixelIndex = 0;
	for (auto& sampleFramebuffer : sampleFramebufferPool) {
		for (auto& pixel : sampleFramebuffer) {
			framebuffer[pixelIndex] = (float)currentSampleCount / (currentSampleCount + 1.f) * framebuffer[pixelIndex] + 1.f / (currentSampleCount + 1.f) * pixel;
			pixelIndex++;
		}
	}
	currentSampleCount++;
}

void Renderer::init(Config* config) {
	clear();
	scene = new Scene();

	scene->loadSceneBox(config);

	camera = new Camera(vec3(278, 273, -790), vec3(278, 273, 0), vec3(0, 1, 0), 38, 1);

	switch (config->accelStructure)
	{
	case ACCEL_NONE: scene->buildNaive(); break;
	case BVH: scene->buildBVH(); break;
	case SAH: scene->buildSAH(); break;
	default:
		break;
	}

}

void Renderer::setModel(const mat4& _model) { model = _model; }
void Renderer::setView(const mat4& _view) { view = _view; }
void Renderer::setPorjection(const mat4& _projection) { projection = _projection; }