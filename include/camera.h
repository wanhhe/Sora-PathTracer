#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "common.h"
#include "ray.h"

class Camera {
public:
	Camera() = default;
	Camera(vec3 lookFrom, vec3 lookTarget, vec3 up, float fov, float aspectRation);
	Ray getRayRandom(int row, int col);

	float deltaTime;
	float speed;
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;

private:
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 upperLeftCorner;
};

#endif // !PATHTRACER_CAMERA_H
