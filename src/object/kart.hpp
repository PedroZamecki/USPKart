#ifndef KART_HPP
#define KART_HPP

#include "object.hpp"

class Wheel final : public Object
{
	bool inverted{false};
	float steeringAngle{0.0f};

public:
	explicit Wheel(const Position &pos, const bool inverted = false) :
		Object("assets/models/wheel.obj", pos), inverted(inverted)
	{
	}

	void steer(const float value) { steeringAngle += value; }

	glm::mat4 getModel(const glm::mat4 &baseModel) override
	{
		auto m_model = baseModel;
		m_model = glm::translate(m_model, pos.toVec3());
		m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));
		m_model = glm::rotate(m_model, steeringAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::rotate(m_model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		m_model = glm::rotate(m_model, inverted ? glm::radians(180.0f) : 0 + angle, glm::vec3(0.0f, 1.0f, 0.0f));
		m_model = glm::rotate(m_model, yaw, glm::vec3(0.0f, 0.0f, 1.0f));
		return m_model;
	}

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel) override
	{

		shader.setMat4("model", getModel(baseModel));
		model.draw(shader);
	}
};

class Kart final : public Object
{
	float speed{0.0f};
	Wheel wheels[4] = {Wheel(pos + Position{10, 4, 12.5} / 16), Wheel(pos + Position{10, 4, -12.5} / 16),
					   Wheel(pos + Position{-10, 4, 12.5} / 16, true), Wheel(pos + Position{-10, 4, -12.5} / 16, true)};

public:
	Kart() : Object("assets/models/kart.obj", Position{0, 0, 0}) {}

	void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel) override
	{
		Object::draw(shader, deltaTime, baseModel);
		for (auto &wheel : wheels)
		{
			wheel.adjustPitch(speed * deltaTime * 0.1f);
			wheel.draw(shader, deltaTime, getModel(baseModel));
		}
	}
};

#endif // KART_HPP
