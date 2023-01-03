#include "light.h"

Light::Light(vec3 _position, vec3 _color, vec3 _intensity) {
	position = _position;
	color = _color;
	_intensity = intensity;
	surround = false;
	surroundRadius = 5.0f;
	surroundCenter = vec3(0.f);
	shape = SQUARE;
	radius = 1.f;
}