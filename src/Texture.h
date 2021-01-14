#ifndef _TEXTURE_H
#define _TEXTURE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>


#include <iostream>
#include <string>
#include <vector>

/**
 *\brief Controls texture objects. Needed because textures need to be glDestroyed.
 *
 *Can only be created through default constructor, because
 *OpenGL encapsulates glTexture objects within the API. Memory management for glTextures is not controlled by the user -
 *this makes copy and copy assignment constructors dangerous to use, so they are deleted.
 **/
class Texture {
	friend class Skybox;
	friend class Model;
	friend class Mesh;
public:
	///2D Texture from path.
	Texture(std::string _path, std::string _typeName, bool _gamma=false);
	///2D Texture from embedded texture.
	Texture(const aiTexture* _texture, std::string _typeName, bool _gamma = false);
	///Cubemap texture, empty.
	Texture(int _width, int _height, std::string _mode);
	///Calls glDestroy on the current texture.
	~Texture();
	//deleting the copy ctors because we need a custom destructor.
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	static unsigned int loadCubemap(std::vector<std::string> _textureFaces);
	unsigned int id() { return id_; }
private:
	unsigned int id_;
	std::string type_;
	//TODO: this is a resource
	std::string path_;
};
#endif
