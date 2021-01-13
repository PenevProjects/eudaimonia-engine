#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Resource.h"

class Shader;
struct Transform;
class Mesh;
class Texture;

class Model : public IResource
{
	friend class Texture;
public:
	Model(std::string _path);
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<std::shared_ptr<Texture>> m_texturesLoaded;
private:
	void ImportModel(std::string _path);
	void ProcessNode(aiNode* _node, const aiScene* _scene);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(const aiScene* _scene, aiMaterial* _mat);
	std::vector<std::shared_ptr<Texture>> LoadTexturesOfType(const aiScene* scene, aiMaterial *mat, aiTextureType type, std::string typeName);

};