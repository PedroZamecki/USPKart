#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glm/glm.hpp>
#include <shader/shader.hpp>
#include <string>
#include <vector>

class Skybox
{
public:
	explicit Skybox(const std::vector<std::string> &faces);
	void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

private:
	Shader skyboxShader{"assets/shaders/skybox.vs", "assets/shaders/skybox.fs"};
	unsigned int skyboxVAO{}, skyboxVBO{};
	unsigned int cubemapTexture;
};

#endif // SKYBOX_HPP
