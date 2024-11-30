#ifndef KART_HPP
#define KART_HPP

#include "object.hpp"
#include "physicsObject.hpp"

class Wheel final : public Object
{
	bool inverted{false};
	float steeringAngle{0.0f};

public:
	explicit Wheel(const Position &pos, const bool inverted = false) :
		Object("assets/models/wheel.obj", pos), inverted(inverted)
	{
	}

	void adjustPitch(const float value) override { angle.x += value; }
	void steer(const float value) { steeringAngle = value; }

	glm::mat4 getModel(const glm::mat4 &baseModel) override
	{
		auto m_model = baseModel;
		m_model = glm::translate(m_model, pos);
		m_model = glm::scale(m_model, glm::vec3(scale.x, scale.y, scale.z));
		m_model = glm::rotate(m_model, steeringAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::rotate(m_model, angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_model = glm::rotate(m_model, (inverted * glm::radians(180.0f)) + angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::rotate(m_model, angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return m_model;
	}

	[[nodiscard]] float getSteeringAngle() const { return steeringAngle; }
};

class Kart : public PhysicsObject
{
protected:
	float steeringAngle{0.0f};
	Wheel frontWheels[2] = {Wheel(Position{-13.6, 1.2, 13.1} / 16), Wheel(Position{13.6, 1.2, 13.1} / 16, true)};
	Wheel backWheels[2] = {Wheel(Position{-13.6, 1.2, -10.9} / 16), Wheel(Position{13.6, 1.2, -10.9} / 16, true)};
	Wheel *wheels[4] = {&frontWheels[0], &frontWheels[1], &backWheels[0], &backWheels[1]};

public:
	Kart(const Position &pos = {0, 0, 0}, const glm::vec3 angle = glm::vec3{0}, const glm::vec3 scale = {1, 1, 1}) :
		PhysicsObject("assets/models/kart.obj", pos, 28.0f / 16.0f, 1, 42.0f / 16.0f, angle, scale)
	{
	}

	[[nodiscard]] float getSteeringAngle() const { return steeringAngle; }
	[[nodiscard]] float getSpeed() const { return glm::dot(velocity, forward()); }

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel, const bool drawBoxes,
			  const Shader &boxShader, const glm::vec3 &maskedColor = {0, 0, 0},
			  const glm::vec3 &maskColor = {0, 0, 0}) override
	{
		Object::draw(shader, deltaTime, baseModel, drawBoxes, boxShader, maskedColor, maskColor);
		for (auto &wheel : frontWheels)
		{
			wheel.steer(steeringAngle);
		}
		for (const auto &wheel : wheels)
		{
			wheel->adjustPitch(getSpeed() * deltaTime);
			wheel->draw(shader, deltaTime, getModel(baseModel), false, boxShader, maskedColor, maskColor);
		}
	}
};

#endif // KART_HPP
