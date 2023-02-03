#ifndef PATHTRACER_MESH_H
#define PATHTRACER_MESH_H

#include <nanogui/glutil.h>
#include "common.h"

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec3 smoothNormal;
	vec2 texcoords;
	vec3 tangent;
	vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	bool isFace;
	bool isHair;

	Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures, bool _isFace, bool _isHair);
	void draw(nanogui::GLShader shader);
	void drawHairShadow(nanogui::GLShader shader);
	void drawFaceStencil(nanogui::GLShader& shader);
	void drawHairShadowStencil(nanogui::GLShader& shader);
	void drwaFaceAfterSampleHairShadowStencil(nanogui::GLShader& shader);

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

#endif // !PATHTRACER_MESH_H


