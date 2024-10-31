#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "object.hpp"

class PhysicsObject : public Object
{
protected:
	glm::vec3 velocity{0};
	glm::vec3 acceleration{0};

public:
	PhysicsObject(const std::string &modelPath, const Position &pos, const float width = 1, const float height = 1,
				  const float depth = 1, const glm::vec3 angle = {0, 0, 0}, const float scale = 1) :
		Object(modelPath, pos, width, height, depth, angle, scale)
	{
	}

	void move(const Position &value) override { pos += {value.x(), 0, value.z()}; }
	void resize(const float value) override { scale += value; }
	void adjustPitch(const float value) override { angle.y += value; }
	glm::vec3 getVelocity() const { return velocity; }

	void treatCollision(Object &other)
	{
		// Get collision data
		const auto [penetration, normal, point] = box.getCollisionForce(other.getBox());

		if (penetration > 0)
		{
			// Calculate the collision force
			const auto collisionForce = normal * -penetration;

			// Move the object in the opposite direction of the collision force
			move(Position(collisionForce * (mass / (mass + other.getMass()))));

			// Move the other object in the opposite direction of the collision force
			other.move(Position(+collisionForce * (other.getMass() / (mass + other.getMass()))));

			// Apply impulse to velocities
			const float restitution = 0.5f; // Bounce factor
			glm::vec3 relativeVelocity = velocity - other.getVelocity();
			float velAlongNormal = glm::dot(relativeVelocity, normal);

			if (velAlongNormal > 0)
			{
				return;
			}

			float j = -(1.0f + restitution) * velAlongNormal;
			j /= (1.0f / mass + 1.0f / other.getMass());

			glm::vec3 impulse = j * normal;
			velocity += impulse / mass;
			other.getVelocity() -= impulse / other.getMass();
		}
	}
};


#endif // PHYSICS_OBJECT_HPP
