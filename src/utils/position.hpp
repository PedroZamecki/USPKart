#ifndef POSITION_HPP
#define POSITION_HPP

#include <cmath>
#include <glm/vec3.hpp>
#include <string>

class Position : public glm::vec3
{
public:
	Position() : glm::vec3() {}
	explicit Position(glm::vec3 vector) : glm::vec3(vector) {}
	Position(float x, float y, float z) : glm::vec3(x, y, z) {}

	// operators
	Position &operator=(const glm::vec3 vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}
	Position &operator=(const Position pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
		return *this;
	}
	bool operator==(const Position pos) const { return x == pos.x && y == pos.y && z == pos.z; }
	bool operator==(const glm::vec3 vector) const { return x == vector.x && y == vector.y && z == vector.z; }
	bool operator!=(const Position pos) const { return x != pos.x || y != pos.y || z != pos.z; }
	bool operator!=(const glm::vec3 vector) const { return x != vector.x || y != vector.y || z != vector.z; }
	Position operator+(const Position pos) const { return {x + pos.x, y + pos.y, z + pos.z}; }
	Position operator+(const glm::vec3 vector) const { return {x + vector.x, y + vector.y, z + vector.z}; }
	Position operator-(const Position pos) const { return {x - pos.x, y - pos.y, z - pos.z}; }
	Position operator-(const glm::vec3 vector) const { return {x - vector.x, y - vector.y, z - vector.z}; }
	Position operator*(const float value) const { return {x * value, y * value, z * value}; }
	Position operator/(const float value) const { return {x / value, y / value, z / value}; }
	Position &operator+=(const Position pos)
	{
		x += pos.x;
		y += pos.y;
		z += pos.z;
		return *this;
	}
	Position &operator+=(const glm::vec3 vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}
	Position &operator-=(const Position pos)
	{
		x -= pos.x;
		y -= pos.y;
		z -= pos.z;
		return *this;
	}
	Position &operator-=(const glm::vec3 vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	void setPos(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	[[nodiscard]] auto toString() const -> std::string
	{
		return "x: " + std::to_string(x) + "; y: " + std::to_string(y) + "; z: " + std::to_string(z) + ";";
	}
	[[nodiscard]] auto norm() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
	[[nodiscard]] auto dist(const Position pos) const -> double
	{
		return sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2) + pow(z - pos.z, 2));
	}

	static auto dist(const Position pos1, const Position pos2) -> double
	{
		return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2));
	}

	[[nodiscard]] Position normalize() const
	{
		const auto value = std::sqrt(x * x + y * y + z * z);
		return {x / value, y / value, z / value};
	}

	[[nodiscard]] float dot(Position pos) const;
	[[nodiscard]] Position cross(Position pos) const;
};

#endif // POSITION_HPP
