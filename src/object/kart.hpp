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
		Object("assets/models/wheel.obj", pos, 0.2, 0.5, 0.5), inverted(inverted)
	{
	}

	void adjustPitch(const float value) override { angle.x += value; }
	void steer(const float value) { steeringAngle = value; }

	glm::mat4 getModel(const glm::mat4 &baseModel) override
	{
		auto m_model = baseModel;
		m_model = glm::translate(m_model, pos.toVec3());
		m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));
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
	Wheel frontWheels[2] = {Wheel(Position{10, -4, 12.5} / 16), Wheel(Position{-10, -4, 12.5} / 16, true)};
	Wheel backWheels[2] = {Wheel(Position{10, -4, -12.5} / 16), Wheel(Position{-10, -4, -12.5} / 16, true)};
	Wheel *wheels[4] = {&frontWheels[0], &frontWheels[1], &backWheels[0], &backWheels[1]};

public:
	explicit Kart(const Position &pos = {0, .5, 0}) :
		PhysicsObject("assets/models/kart.obj", pos, 18.0f / 16.0f, 1, 44.0f / 16.0f)
	{
	}

	[[nodiscard]] float getSteeringAngle() const { return steeringAngle; }
	[[nodiscard]] float getSpeed() const { return glm::dot(velocity, forward()); }

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel, const bool drawBoxes,
			  const Shader &boxShader) override
	{
		Object::draw(shader, deltaTime, baseModel, drawBoxes, boxShader);
		for (auto &wheel : frontWheels)
		{
			wheel.steer(steeringAngle);
		}
		for (const auto &wheel : wheels)
		{
			wheel->adjustPitch(getSpeed() * deltaTime);
			wheel->draw(shader, deltaTime, getModel(baseModel), false, boxShader);
		}
	}
};

#endif // KART_HPP
