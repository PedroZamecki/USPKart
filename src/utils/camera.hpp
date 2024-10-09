#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <mutex>

#include "game/config.hpp"
#include "position.hpp"

class Camera
{
	static Camera *instance;
	static std::mutex mtx;
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
	Position pos{20, 5, 20}, target{0, 0, 0};;
	float pitch{0}, yaw{0}, roll{0}, fov{80}, near{0.1}, far{4096};

	void setPos(const Position &pos) { this->pos = pos; }

	static float aspectRatio()
	{
		const auto config = Configuration::getInstance();
		return static_cast<float>(config->width) / static_cast<float>(config->height);
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
