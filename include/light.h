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
	// Ĭ�ϲ����ƣ�����
	Light(const string& _name, int _type, vec3 _position, vec3 _color, vec3 _intensity);
	vec3 position;
	vec3 color;
	vec3 intensity;

	// ����
	bool surround;
	float surroundRadius;
	vec3 surroundCenter;

	// ��Դ��״
	float radius;
	LightShapeEnum shape;

	// ��Դ����
	string name;
	// ��Դ����
	int type;
};

#endif // !PATHTRACER_LIGHT_H
