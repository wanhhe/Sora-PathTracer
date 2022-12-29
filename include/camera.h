#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "common.h"
#include "ray.h"

//class Camera {
//public:
//	Camera() = default;
//	Camera(vec3 lookFrom, vec3 lookTarget, vec3 up, float fov, float aspectRation);
//	Ray getRayRandom(int row, int col);
//
//	float deltaTime;
//	float translateSpeed;
//	float scaleSpeed;
//	float sensitivity;
//	float yaw = -90.f;
//	float pitch = 0;
//	float lastX = 200.f;
//	float lastY = 200.f;
//	float startX;
//	float startY;
//	float fov = 45.0f;
//	float radius = 1.0f;
//	vec3 cameraPos;
//	vec3 cameraFront;
//	vec3 cameraUp;
//	vec3 cameraTarget;
//	bool firstMouse = true;
//
//private:
//	vec3 origin;
//	vec3 horizontal;
//	vec3 vertical;
//	vec3 upperLeftCorner;
//};

class Camera {
public:
	Camera() = default;
	Camera(vec3 _position, vec3 _target, float _aspect) : 
		position(_position), target(_target), aspect(_aspect), up(vec3(0,1,0)), fov(radians(60.0f)), 
		isPanning(false), isOrbiting(false), cameraPosition(vec3(0,0,3.0f)), cameraTarget(vec3(0,0,0)), cursor(vec2(200.f, 200.f)), orbitDelta(vec2(0.f, 0.f)),
	    panDelta(vec2(0.f, 0.f)) {}
	vec3 calculatePan(vec3 fromCamera);
	vec3 calculateOffset(vec3 fromTarget);
	void updateTransform();
	void setTransform(vec3 _position, vec3 _target);
	vec3 getForward();
	void setDefaultLocation(vec3 _cameraPosition, vec3 _cameraTarget);
	void recoverDefaultLocation();

	vec2 orbit;
	vec2 pan;
	float dolly;

	vec3 position;
	vec3 target;
	vec3 up;
	float aspect;
	float fov;

	bool isOrbiting;
	vec2 orbitPos;
	vec2 orbitDelta;

	bool isPanning;
	vec2 panPos;
	vec2 panDelta;

	float dollyDelta = 0.f;

	vec3 cameraPosition;
	vec3 cameraTarget;

	vec2 cursor;
};

#endif // !PATHTRACER_CAMERA_H
