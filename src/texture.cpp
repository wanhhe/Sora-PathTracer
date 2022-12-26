#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"

Texture::Texture(const string& name) {
	data = stbi_load(name.c_str(), &width, &height, &nrChannel, 0);
}

Texture::~Texture() {
	stbi_image_free(data);
}

vec3 Texture::getRGB(float row, float col) {
	auto dx = nrChannel * width * row + nrChannel * height * col;
	return vec3(dx, dx+1, dx+2);
}                

vec4 Texture::getRGBA(float row, float col) {
	auto dx = nrChannel * width * row + nrChannel * height * col;
	return vec4(dx, dx + 1, dx + 2, dx + 3);
}