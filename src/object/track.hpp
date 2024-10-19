#ifndef TRACK_HPP
#define TRACK_HPP
#include "object.hpp"

class Track final : public Object
{
public:
	Track() : Object("assets/models/track.obj") {}

	void draw(const Shader &shader, float deltaTime, const glm::mat4 baseModel) override
	{
		shader.setMat4("model", {1});
		model.draw(shader);
	}
};

#endif // TRACK_HPP
