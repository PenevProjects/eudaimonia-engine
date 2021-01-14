#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteVertexArrays(1, &vbo_);
	glDeleteVertexArrays(1, &ebo_);
}


Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<std::shared_ptr<Texture>> _textures, Colors _material)
{
	this->vertices_ = _vertices;
	this->indices_ = _indices;
	this->textures_ = _textures;
	this->colors_ = _material;

	setupMeshVAO();
}

Mesh::Mesh(aiMesh *mesh, const aiScene *scene, std::vector<std::shared_ptr<Texture>>& _textures)
{

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 tempVec; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		tempVec.x = mesh->mVertices[i].x;
		tempVec.y = mesh->mVertices[i].y;
		tempVec.z = mesh->mVertices[i].z;
		vertex.position = tempVec;
		// normals
		if (mesh->HasNormals())
		{
			tempVec.x = mesh->mNormals[i].x;
			tempVec.y = mesh->mNormals[i].y;
			tempVec.z = mesh->mNormals[i].z;
			vertex.normal = tempVec;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texUVs = vec;
		}
		else {
			vertex.texUVs = glm::vec2(0.0f, 0.0f);
		}
		if (mesh->HasTangentsAndBitangents())
		{
			tempVec.x = mesh->mTangents[i].x;
			tempVec.y = mesh->mTangents[i].y;
			tempVec.z = mesh->mTangents[i].z;
			vertex.tangents = tempVec;

			tempVec.x = mesh->mBitangents[i].x;
			tempVec.y = mesh->mBitangents[i].y;
			tempVec.z = mesh->mBitangents[i].z;
			vertex.bitangents = tempVec;
		}
		vertices_.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			this->indices_.push_back(face.mIndices[j]);
	}


	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];




	//set colors of model in case it has no textures
	aiColor3D color(0.f, 0.f, 0.f);
	Colors tempColors;
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	colors_.diffuse = glm::vec3(color.r, color.b, color.g);

	material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	colors_.ambient = glm::vec3(color.r, color.b, color.g);

	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	colors_.specular = glm::vec3(color.r, color.b, color.g);


	//add textures to textures vec
	for (auto& texture : _textures)
	{
		textures_.push_back(texture);
	}

	setupMeshVAO();
}

void Mesh::setupMeshVAO()
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
		&indices_[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texUVs));
	// vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangents));
	// vertex bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangents));

	glBindVertexArray(0);
}

void Mesh::render(const Shader &_shader)
{
	//for models without textures
	//_shader.setBool("hasTextures", false);
	//_shader.setVec3("material.diffuse", this->m_colors.diffuse);
	//_shader.setVec3("material.specular", this->m_colors.specular);
	//_shader.setVec3("material.ambient", this->m_colors.ambient);
	if (!textures_.empty())
	{
		//_shader.setBool("hasTextures", true);
		for (unsigned int i = 0; i < textures_.size(); i++)
		{
			// retrieve texture name
			std::string name = textures_[i]->type_;
			//set the id of the sampler
			_shader.setInt(("material." + name).c_str(), i);
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			glBindTexture(GL_TEXTURE_2D, textures_[i]->id_);
		}
	}
	// draw mesh
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

