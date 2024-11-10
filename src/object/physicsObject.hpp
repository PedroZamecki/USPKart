#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "object.hpp"
#include "utils/logger.hpp"

class PhysicsObject : public Object
{
	bool isValidVector(glm::vec3 vector)
	{
		return !std::isnan(vector.x) && !std::isnan(vector.y) && !std::isnan(vector.z);
	}

protected:
	glm::vec3 velocity{0};
	glm::vec3 acceleration{0};
	glm::vec3 angularVelocity{0};

public:
	PhysicsObject(const std::string &modelPath, const Position &pos, const float width = 1, const float height = 1,
				  const float depth = 1, const glm::vec3 angle = {0, 0, 0}, const glm::vec3 scale = {1, 1, 1}) :
		Object(modelPath, pos, width, height, depth, angle, scale)
	{
	}

	glm::vec3 getAngularVelocity() { return angularVelocity; }
	glm::vec3 *getAcceleration() override { return &acceleration; }
	glm::vec3 *getVelocity() override { return &velocity; }
	void move(const glm::vec3 &value) override { pos += value; }
	void resize(const float value) override { scale *= value; }
	void rotate(const float value) override { angle.y += value; }
	float getMass() override { return mass; }

	void resolveCollision(Object &other, const glm::vec3 &collisionForce, const glm::vec3 &normal, float penetration)
	{
		float otherMass = other.getMass();
		float totalMass = mass + otherMass;
		float massRatioThis = otherMass / totalMass;
		float massRatioOther = mass / totalMass;

		glm::vec3 correctionForce = normal * penetration;

		move(correctionForce * massRatioThis * -1.0f);
		other.move(correctionForce * massRatioOther);

		glm::vec3 relativeVelocity = velocity - *other.getVelocity();
		float velocityAlongNormal = glm::dot(relativeVelocity, normal);

		if (velocityAlongNormal > 0)
			return;

		// Elastic collision
		constexpr float e = 0.5f;
		float j = -(1 + e) * velocityAlongNormal;
		j /= 1 / mass + (otherMass ? 1 / otherMass : 0);

		glm::vec3 impulse = normal * j;
		velocity += impulse / mass;
		if (otherMass)
			*other.getVelocity() += impulse / otherMass;
	}

	void treatCollision(Object &other)
	{
		const auto collisionData = box.getCollisionForce(other.getBox());
		if (collisionData.penetration > 0)
		{
			const auto collisionForce = collisionData.normal * collisionData.penetration;
			resolveCollision(other, collisionForce, collisionData.normal, collisionData.penetration);
		}
	}

	void applyFriction(float deltaTime)
	{
		glm::vec3 frictionForce = -velocity * 0.1f;
		acceleration += frictionForce / mass;
	}

	void updatePhysics(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		pos += velocity * deltaTime;
		angularVelocity += acceleration * deltaTime;
		angle += angularVelocity * deltaTime;
		applyFriction(deltaTime);
		acceleration = glm::vec3(0);
	}

	void draw(const Shader &shader, float deltaTime, glm::mat4 baseModel, bool drawBoxes,
			  const Shader &boxShader) override
	{
		updatePhysics(deltaTime);
		Object::draw(shader, deltaTime, baseModel, drawBoxes, boxShader);
	}
};


#endif // PHYSICS_OBJECT_HPP
