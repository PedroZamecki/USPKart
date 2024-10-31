#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "object.hpp"

class PhysicsObject : public Object
{
protected:
	glm::vec3 velocity{0};
	glm::vec3 acceleration{0};
	float mass{1};

public:
	PhysicsObject(const std::string &modelPath, const Position &pos, const float width = 1, const float height = 1,
				  const float depth = 0, const glm::vec3 angle = {0, 0, 0}, const float scale = 1) :
		Object(modelPath, pos, width, height, depth, angle, scale)
	{
	}

	void move(const Position &value) override { pos += value; }
	void resize(const float value) override { scale += value; }
	void adjustPitch(const float value) override { angle.y += value; }

	void treatCollision(const Object &other) override
	{
		// Check if the objects are close enough to potentially collide
		if (glm::distance(pos.toVec3(), other.getPos().toVec3()) > (width + other.getWidth()))
			return;

		// Get collision data
		if (const auto [penetration, vector, point] = box.getCollisionForce(other.getBox()); penetration > 0)
		{
			// Calculate the collision force
			const auto force = vector * penetration;

			// Update velocity based on the collision force
			velocity += force / mass;

			// Update position to resolve penetration
			pos += static_cast<Position>(vector * penetration);

			// Apply friction to simulate deceleration
			constexpr auto frictionCoefficient = 0.1f;
			const auto friction = glm::normalize(velocity) * frictionCoefficient * glm::length(force);
			velocity -= friction;
		}
	}
};


#endif // PHYSICS_OBJECT_HPP
