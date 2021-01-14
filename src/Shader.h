#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>


class Shader
{
private:
	///Holds shader program ID.
	GLuint shader_program_;
public:
	/**
	*\brief Creates a new shader. 
	*
	*@param vertexPath Path to vertex shader file.
	*@param fragmentPath Path to fragment shader file.
	*/
	Shader(const GLchar* vertex_path, const GLchar* fragment_path);
	///Binds current shader object
	void use();
	///Unbinds current shader object
	void stopUsing();
	void setBool(const std::string &name, GLboolean value) const
	{
		glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(shader_program_, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(shader_program_, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(shader_program_, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(shader_program_, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(shader_program_, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(shader_program_, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(shader_program_, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(shader_program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(shader_program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};

#endif