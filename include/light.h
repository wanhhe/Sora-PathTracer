#ifndef PATHTRACER_LIGHT_H
#define PATHTRACER_LIGHT_H

#include "common.h"

class Light {
public:
	Light() {
		name = "";
		position = vec3(-2.f);
		color = vec3(1.0f);
		intensity = vec3(10.f);
		surround = false;
		surroundRadius = 5.0f;
		surroundCenter = vec3(0.f);
		shape = SQUARE;
		radius = 1.f;
	}
	// 默认不环绕，方形
	Light(const string& _name, int _type, vec3 _position, vec3 _color, vec3 _intensity);
	vec3 position;
	vec3 color;
	vec3 intensity;

	// 环绕
	bool surround;
	float surroundRadius;
	vec3 surroundCenter;

	// 光源形状
	float radius;
	LightShapeEnum shape;

	// 光源名称
	string name;
	// 光源类型
	int type;
};

#endif // !PATHTRACER_LIGHT_H
