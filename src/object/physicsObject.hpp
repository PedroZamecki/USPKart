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
	float getMass() override { return mass; }

	void treatCollision(Object &other)
	{
		if (mass == 0)
			return;
		// Get collision data
		const auto [penetration, normal, point] = box.getCollisionForce(other.getBox());

		if (penetration > 0)
		{
			// Calculate the collision force
			const auto collisionForce = normal * -penetration;
			const auto otherMass = other.getMass();
			const auto massesSum = otherMass + mass;
			const auto proportion = otherMass ? mass / massesSum : 1;
			const auto otherProportion = -otherMass / massesSum;

			// Move the objects in the opposite direction of the collision force
			move(Position(collisionForce * proportion));
			other.move(Position(collisionForce * otherProportion));

			// Apply impulse to velocities
			const float restitution = 0.5f; // Bounce factor
			glm::vec3 relativeVelocity = velocity - other.getVelocity();
			float velAlongNormal = glm::dot(relativeVelocity, normal);

			if (velAlongNormal > 0)
			{
				return;
			}

			float j = -(1.0f + restitution) * velAlongNormal;
			j /= otherMass ? (1 / mass + 1 / otherMass) : 1 / mass;

			glm::vec3 impulse = j * normal;
			velocity += impulse / mass;
			other.getVelocity() -= otherMass ? (impulse / otherMass) : glm::vec3{0};
		}
	}
};


#endif // PHYSICS_OBJECT_HPP
