#ifndef _SKYBOX_H
#define _SKYBOX_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <memory>

#include "Texture.h"
#include "FrameBuffer.h"

class Skybox
{
	friend class Texture;
public:
	/**\brief Creates an environment skybox from an HDR equirectangular image.
	*
	* @param _pathToHDR Path to .hdr file.
	* @param _size Specified width and height of render texture for the cubemap dimensions.
	*/
	Skybox(std::string _pathToHDR, unsigned int _size);
	~Skybox();
	Skybox(const Skybox&) = delete;
	Skybox& operator=(const Skybox&) = delete;
	void renderCube();
	std::weak_ptr<Texture> environment_map() { return environment_map_; }
	std::weak_ptr<Texture> irradiance_map() { return irradiance_map_; }
	std::weak_ptr<Texture> prefilter_map() { return prefilter_map_; }
	std::weak_ptr<Texture> brdf_lut() { return brdf_lut_; }

private:
	std::shared_ptr<Texture> environment_map_;
	std::shared_ptr<Texture> irradiance_map_;
	std::shared_ptr<Texture> prefilter_map_;
	std::shared_ptr<Texture> brdf_lut_;

	std::shared_ptr<FrameBuffer> m_framebuffer;
	//todo:
	//void transformHDRtoCubemap();	
	//void generateEnvironmentMap();
	//void generateIrradianceMap();
	//void generatePrefilterMap();
	//void generateBrdfLUT();
	void createCubeVAO();

	unsigned int vao_;
	unsigned int size_;
	std::string path_;

	glm::mat4 capture_projection_;
	glm::mat4 capture_view_[6];
};
#endif