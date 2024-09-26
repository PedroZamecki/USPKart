#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/fwd.hpp>
#include <string>

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char *vertexPath, const char *fragmentPath);
	// activate the shader
	// ------------------------------------------------------------------------
	void use() const;
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;

	void setMat4(const std::string &name, const glm::mat4 &value) const;
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	static void checkCompileErrors(unsigned int shader, const std::string &type);
};

#endif // SHADER_HPP
