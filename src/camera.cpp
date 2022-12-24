#include "camera.h"

Camera::Camera(vec3 lookFrom, vec3 lookTarget, vec3 up, float fov, float aspectRation) {
	float theta = radians(fov);
	auto h = tan(theta / 2.0);
	auto viewHeight = 2.0 * h;
	auto viewWidth = viewHeight * aspectRation;

	// ÉãÏñ»úµÄ×ø±ê u,v,w
	auto w = normalize(lookFrom - lookTarget);
	auto u = normalize(cross(up, w));
	auto v = cross(w, u);

	origin = lookFrom;
	horizontal = (float)viewWidth * u;
	vertical = (float)viewHeight * v;
	upperLeftCorner = origin - w - horizontal / 2.f - vertical / 2.f;
}

Ray Camera::getRayRandom(int row, int col) {
	auto sample = linearRand(vec2(0.0), vec2(1.0));
	float rowScale = (float)row + sample.x;
	float colScale = (float)col + sample.y;
	auto lookAt = upperLeftCorner + vertical * rowScale / (float)SCREEN_HEIGHT + horizontal * colScale / (float)SCREEN_WIDTH;
	return Ray(origin, lookAt - origin);
}