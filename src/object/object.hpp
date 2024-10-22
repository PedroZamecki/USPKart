#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "collisionBox.hpp"
#include "model/model.hpp"

class Object
{
protected:
	Position pos{0, 0, 0};
	Model model;
	float width{1}, height{1}, depth{1}, pitch{0}, angle{0}, yaw{0}, scale{1};
	CollisionBox box = CollisionBox(&pos, width, height, depth);

public:
	virtual glm::mat4 getModel(const glm::mat4 &baseModel)
	{
		auto m_model = baseModel;
		m_model = glm::translate(m_model, pos.toVec3());
		m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));
		m_model = glm::rotate(m_model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		m_model = glm::rotate(m_model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::rotate(m_model, yaw, glm::vec3(0.0f, 0.0f, 1.0f));
		return m_model;
	}

	void move(const Position &value) { pos += value; }

	void resize(const float value) { scale += value; }

	void rotate(const float value) { angle += value; }

	void adjustPitch(const float value) { pitch += value; }

	explicit Object(const std::string &modelPath, Position pos = {0, 0, 0}, const float width = 1,
					const float height = 1, const float depth = 1, const float pitch = 0, const float angle = 0,
					const float yaw = 0, const float scale = 1) :
		pos(pos), model(modelPath), width(width), height(height), depth(depth), pitch(pitch), angle(angle), yaw(yaw),
		scale(scale)
	{
		box = CollisionBox(&pos, width, height, depth);
	}

	explicit Object(const Model &model, Position pos = {0, 0, 0}, const float width = 1, const float height = 1,
					const float depth = 1, const float pitch = 0, const float angle = 0, const float yaw = 0,
					const float scale = 1) :
		pos(pos), model(model), width(width), height(height), depth(depth), pitch(pitch), angle(angle), yaw(yaw),
		scale(scale)
	{
		box = CollisionBox(&pos, width, height, depth);
	}

	virtual ~Object() = default;

	virtual void draw(const Shader &shader, float deltaTime, const glm::mat4 baseModel)
	{
		shader.setMat4("model", getModel(baseModel));
		model.draw(shader);
	}
};

#endif // OBJECT_HPP
