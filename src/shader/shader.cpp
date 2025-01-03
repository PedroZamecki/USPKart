#include "shader.hpp"

#include <GL/glew.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <utils/logger.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	const auto logger = Logger::getInstance();
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		if (!vShaderFile.is_open() || !fShaderFile.is_open())
		{
			logger->error("Failed to open shader file");
			throw std::runtime_error("Failed to open shader file");
			return;
		}
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		logger->error("Shader file not successfully read: " + std::string(e.what()));
		return;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const
{
	glUseProgram(ID);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setBool(const std::string &name, const bool value) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setInt(const std::string &name, const int value) const
{
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setFloat(const std::string &name, const float value) const
{
	use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	use();
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec2(const std::string &name, const float x, const float y) const
{
	use();
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	use();
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec3(const std::string &name, const float x, const float y, const float z) const
{
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	use();
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setVec4(const std::string &name, const float x, const float y, const float z, const float w) const
{
	use();
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	use();
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	use();
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	if (const auto err = glGetError())
	{
		Logger::getInstance()->error("OpenGL error: (" + std::to_string(err) + ") - " +
									 reinterpret_cast<const char *>(glewGetErrorString(err)));
		throw std::runtime_error("OpenGL error");
	}
}

void Shader::checkCompileErrors(const unsigned int shader, const std::string &type)
{
	int success;
	char infoLog[1024];
	const auto logger = Logger::getInstance();
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			logger->error("Shader compilation error of type: " + type + ": " + infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			logger->error("Shader linking error of type: " + type + ": " + infoLog);
		}
	}
}
