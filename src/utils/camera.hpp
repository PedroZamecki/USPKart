#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <mutex>

#include "position.hpp"

class Camera
{
	static Camera *instance;
	static std::mutex mtx;

	Camera();
	Camera(Position pos, Position target, float pitch, float yaw, float roll, float fov, float aspectRatio, float near,
		   float far);
public:
	static Camera *getInstance()
	{
		if (instance == nullptr)
		{
			std::lock_guard lock(mtx);
			if (instance == nullptr)
			{
				instance = new Camera();
			}
		}
		return instance;
	}
	Position pos, target;
	float pitch, yaw, roll, fov, aspectRatio, near, far;

	void setAspectRatio(const int width, const int height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	[[nodiscard]] auto targetDist() const -> auto { return (target - pos).norm(); }

	[[nodiscard]] auto forward() const -> glm::vec<3, float>
	{
		return {std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch)};
	}
	[[nodiscard]] auto right() const -> glm::vec<3, float> { return cross(forward(), glm::vec<3, float>({0, 1, 0})); }
	[[nodiscard]] auto up() const -> glm::vec<3, float>
	{
		// Calculate the cross product between the forward vector and {0, 1, 0}
		return cross(right(), forward());
	}
	[[nodiscard]] auto direction() const -> glm::vec<3, float>
	{
		return {cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))};
	}
	[[nodiscard]] auto cameraFront() const -> auto { return normalize(direction()); };

	[[nodiscard]] glm::mat4 getViewMatrix() const;

	[[nodiscard]] glm::mat4 getProjectionMatrix() const;
};

#endif // CAMERA_HPP
