#ifndef _MODEL_RESOURCE_H
#define _MODEL_RESOURCE_H

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
	std::vector<std::shared_ptr<Mesh>> meshes_;
	std::vector<std::shared_ptr<Texture>> textures_loaded_;
private:
	void importModel(std::string _path);
	void processNode(aiNode* _node, const aiScene* _scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(const aiScene* _scene, aiMaterial* _mat);
	std::vector<std::shared_ptr<Texture>> loadTexturesOfType(const aiScene* _scene, aiMaterial *_mat, aiTextureType type, std::string typeName);

};
#endif