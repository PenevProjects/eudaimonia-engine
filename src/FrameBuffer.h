#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H


class Shader;

class FrameBuffer {
public:
	/** \brief Framebuffer ctor.
	 *
	 * Constructs a FrameBuffer object and creates its render texture and render buffer object.
	 * @param _width Dimension of framebuffer width.
	 * @param _height Dimension of framebuffer height.
	 **/
	FrameBuffer::FrameBuffer(int _width, int _height);
	~FrameBuffer();
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	//Draws a quad. No view or projection matrix.
	void drawQuad();
	unsigned int fbo() { return fbo_; }
	unsigned int texture() { return texture_id_; }
	unsigned int rbo() { return rbo_; }
private:
	void createRenderTexture();
	void createRenderBuffer();
	void createRenderQuad();

	unsigned int m_screenQuadVAO;
	unsigned int fbo_;
	unsigned int rbo_;
	unsigned int texture_id_;
	int width_;
	int height_;
};
#endif