#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "collisionBox.hpp"
#include "model/model.hpp"
#include "utils/position.hpp"

class Object
{
protected:
	Position pos{0, 0, 0};
	const Model *model;
	float width{1}, height{1}, depth{1}, mass{1};
	glm::vec3 scale{1};
	glm::vec3 angle{0};
	CollisionBox box = CollisionBox(&pos, &angle, &scale, &width, &height, &depth);

public:
	virtual glm::mat4 getModel(const glm::mat4 &baseModel)
	{
		auto m_model = baseModel;
		m_model = translate(m_model, pos);
		m_model = glm::scale(m_model, glm::vec3(scale.x, scale.y, scale.z));
		m_model = glm::rotate(m_model, angle.x, glm::vec3{1, 0, 0});
		m_model = glm::rotate(m_model, angle.y, glm::vec3{0, 1, 0});
		m_model = glm::rotate(m_model, angle.z, glm::vec3{0, 0, 1});
		return m_model;
	}

	[[nodiscard]] glm::vec3 getAngle() const { return angle; }
	[[nodiscard]] CollisionBox getBox() const { return box; }
	[[nodiscard]] float getHeight() const { return height; }
	[[nodiscard]] float getWidth() const { return width; }
	[[nodiscard]] float getDepth() const { return depth; }
	[[nodiscard]] Position getPos() const { return pos; }

	// Do nothing by default
	virtual void applyCorrectionForce(const glm::vec3 &correctionForce, float massRatio) {}
	virtual glm::vec3 getAngularVelocity() { return {0, 0, 0}; }
	virtual glm::vec3 *getAcceleration() { return nullptr; }
	virtual glm::vec3 *getVelocity() { return nullptr; }
	virtual void adjustPitch(const float value) {}
	virtual void treatCollision(Object &other) {}
	virtual void move(const glm::vec3 &value) {}
	virtual void resize(const float value) {}
	virtual void rotate(const float value) {}
	virtual float getInertia() { return 0; }
	virtual float getMass() { return 0; }

	explicit Object(const std::string &modelPath, const Position &pos = {0, 0, 0}, const float width = 1,
					const float height = 1, const float depth = 1, const glm::vec3 angle = {0, 0, 0},
					const glm::vec3 scale = {1, 1, 1}) :
		pos(pos), model(ResourceManager::getInstance()->loadModel(modelPath)), width(width), height(height),
		depth(depth), angle(angle), scale(scale),
		box(CollisionBox(&this->pos, &this->angle, &this->scale, &this->width, &this->height, &this->depth))
	{
	}

	virtual ~Object() = default;

	[[nodiscard]] glm::vec3 forward() const
	{
		// Calculate the forward vector
		// pitch = angle.x
		// yaw = angle.y
		// roll = angle.z

		return {std::cos(angle.x) * std::sin(angle.y), -std::sin(angle.x), std::cos(angle.x) * std::cos(angle.y)};
	}

	[[nodiscard]] glm::vec3 right() const
	{
		// Calculate the right vector
		// pitch = angle.x
		// yaw = angle.y
		// roll = angle.z

		return {std::cos(angle.y), 0, -sin(angle.y)};
	}

	virtual void draw(const Shader &shader, float deltaTime, const glm::mat4 baseModel, const bool drawBoxes,
					  const Shader &boxShader)
	{
		shader.use();
		shader.setMat4("model", getModel(baseModel));
		model->draw(shader);
		if (drawBoxes)
		{
			box.draw(boxShader);
		}
	}
};

#endif // OBJECT_HPP
