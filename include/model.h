#ifndef PATHTRACER_MODEL_H
#define PATHTRACER_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model {
public:
	Model(const string& path, const string& _name);
	void draw(nanogui::GLShader& shader);

	vec3 translate;
	vec3 scale;
	string name;
	int shaderIndex;
	float roughenss;
	float metallic;

private:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;
	bool gammaCorrection = false;

	void loadModel(const string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* _material, aiTextureType type, const string& typeName);
	unsigned int textureFromFile(const char* path, const string& directory, bool gamma = false);
};

#endif // !PATHTRACER_MODEL_H
