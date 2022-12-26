#ifndef PATHTRACER_TEXTURE_H
#define PATHTRACER_TEXTURE_H

#include "common.h"

class Texture {
public:
	Texture(const string& name);
	~Texture();
	vec3 getRGB(float row, float col);
	vec4 getRGBA(float row, float col);
	int getChannel() const { return nrChannel; }
private:
	int width;
	int height;
	int nrChannel;
	unsigned char* data;
};

#endif // !PATHTRACER_TEXTURE_H
