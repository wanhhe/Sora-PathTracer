#include "camera.h"

Camera::Camera(vec3 lookFrom, vec3 lookTarget, vec3 up, float fov, float aspectRation) {
	auto theta = radians(fov);
	auto viewHeight = 2.0;
	auto viewWidth = viewHeight * aspectRation;

	// ÉãÏñ»úµÄ×ø±ê u,v,w
	auto w = normalize(lookFrom - lookTarget);
	auto u = normalize(cross(up, w));
	auto v = cross(w, u);
	origin = lookFrom;
}