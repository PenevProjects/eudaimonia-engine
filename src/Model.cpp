#include "Model.h"
#include "Shader.h"
#include "Transform.h"
#include <iostream>
#include "Mesh.h"
#include "Texture.h"


Model::Model(std::string _path) :
	IResource(_path)
{
	importModel(_path);
}


void Model::importModel(std::string _path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	this->directory_ = _path.substr(0, _path.find_last_of('/'));
	std::cout << "\nImporting model: " << _path;

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *_node, const aiScene *_scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < _node->mNumMeshes; i++)	
	{
		aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
		aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> meshTextures = loadMaterialTextures(_scene, material);
		std::shared_ptr<Mesh> currentMesh = std::make_shared<Mesh>(mesh, _scene, meshTextures);
		std::cout << "\nloaded mesh.\n";
		meshes_.push_back(currentMesh);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < _node->mNumChildren; i++)
	{
		processNode(_node->mChildren[i], _scene);
	}
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(const aiScene* _scene, aiMaterial* _mat)
{
	std::vector<std::shared_ptr<Texture>> textures;

	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadTexturesOfType(_scene, _mat, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<std::shared_ptr<Texture>> specularMaps = loadTexturesOfType(_scene, _mat, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<std::shared_ptr<Texture>> normalMaps = loadTexturesOfType(_scene, _mat, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<std::shared_ptr<Texture>> heightMaps = loadTexturesOfType(_scene, _mat, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	std::vector<std::shared_ptr<Texture>> bcMaps = loadTexturesOfType(_scene, _mat, aiTextureType_BASE_COLOR, "texture_albedo");
	textures.insert(textures.end(), bcMaps.begin(), bcMaps.end());
	std::vector<std::shared_ptr<Texture>> normalPbrMaps = loadTexturesOfType(_scene, _mat, aiTextureType_NORMAL_CAMERA, "texture_normal");
	textures.insert(textures.end(), normalPbrMaps.begin(), normalPbrMaps.end());
	std::vector<std::shared_ptr<Texture>> metallicMaps = loadTexturesOfType(_scene, _mat, aiTextureType_METALNESS, "texture_metallic");
	textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
	std::vector<std::shared_ptr<Texture>> roughnessMaps = loadTexturesOfType(_scene, _mat, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
	textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
	std::vector<std::shared_ptr<Texture>> aoMaps = loadTexturesOfType(_scene, _mat, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
	textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
	return textures;
}

std::vector<std::shared_ptr<Texture>> Model::loadTexturesOfType(const aiScene* scene, aiMaterial *mat, aiTextureType _type, std::string _typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	std::cout << "\nTexture type: " << _typeName << " count:" << mat->GetTextureCount(_type);
	if (mat->GetTextureCount(_type) > 1)
	{
		std::cout << "\nMODEL::LoadTexturesOfType::WARNING: More than one texture of type " << _typeName << " detected! This is not currently supported.\n";
	}
	for (unsigned int i = 0; i < mat->GetTextureCount(_type); i++)
	{
		aiString pathToTexture;
		mat->GetTexture(_type, i, &pathToTexture);
		bool loaded = false;
		for (unsigned int j = 0; j < textures_loaded_.size(); j++)
		{
			//if the loaded texture's path(cstring) is the same as the current material texture path
			if (std::strcmp(textures_loaded_[j]->path_.data(), pathToTexture.C_Str()) == 0)
			{
				textures.push_back(textures_loaded_[j]); //push the loaded texture 
				loaded = true;
				break;
			}
		}
		if (!loaded)
		{   // if texture hasn't been loaded already, load it

			//load an embedded texture
			const aiTexture* embedded = scene->GetEmbeddedTexture(pathToTexture.C_Str());

			if (embedded) {
				//construct from embedded
				std::shared_ptr<Texture> texture = std::make_shared<Texture>(embedded, _typeName);
				//push to local texture vector(of uniform types)
				textures.push_back(texture);
				//push to member texture vector(of various types)
				textures_loaded_.push_back(texture);
			}
			else {

				//process path
				std::string filename = pathToTexture.C_Str();
				filename = directory_ + '/' + filename;
				//construct from file path
				std::shared_ptr<Texture> texture = std::make_shared<Texture>(filename, _typeName);
				//push to local texture vector(of uniform types)
				textures.push_back(texture);
				//push to member texture vector(of various types)
				textures_loaded_.push_back(texture);
			}
		}
	}
	return textures;
}