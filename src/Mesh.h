#ifndef _MESH_RESOURCE_H
#define _MESH_RESOURCE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Texture;
class Shader;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texUVs;
	glm::vec3 tangents;
	glm::vec3 bitangents;
};

struct Colors {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
};

class Mesh {
	friend class Texture;
public:
	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	//using shared ptrs to avoid copy ctor
	std::vector<std::shared_ptr<Texture>> textures_;
	Colors colors_;
	
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<std::shared_ptr<Texture>> _textures, Colors _material);
	Mesh(aiMesh *_mesh, const aiScene *_scene, std::vector<std::shared_ptr<Texture>>& _textures);
	~Mesh();
	Mesh& operator=(const Mesh&) = delete;
	Mesh(const Mesh&) = delete;
	
	/**
	 * Binds all textures to a shader and renders a mesh.
	 * @param _shader A shader object passed by const-ref, because we don't want to transfer ownership.
	 */
	void render(const Shader &_shader);

private:
	unsigned int vao_, vbo_, ebo_;

	void setupMeshVAO();
};
#endif