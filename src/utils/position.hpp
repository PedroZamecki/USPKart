#ifndef POSITION_HPP
#define POSITION_HPP

#include <cmath>
#include <glm/vec3.hpp>
#include <string>

class Position
{
public:
	Position();
	explicit Position(glm::vec3 vector);
	Position(float x, float y, float z);

	// operators
	Position& operator=(const glm::vec3 vector) { x_ = vector.x; y_ = vector.y; z_ = vector.z; return *this; }
	Position& operator=(const Position pos) { x_ = pos.x(); y_ = pos.y(); z_ = pos.z(); return *this; }
	bool operator==(const Position pos) const { return x_ == pos.x() && y_ == pos.y() && z_ == pos.z(); }
	bool operator==(const glm::vec3 vector) const { return x_ == vector.x && y_ == vector.y && z_ == vector.z; }
	bool operator!=(const Position pos) const { return x_ != pos.x() || y_ != pos.y() || z_ != pos.z(); }
	bool operator!=(const glm::vec3 vector) const { return x_ != vector.x || y_ != vector.y || z_ != vector.z; }

	void setPos(float x, float y, float z);

	Position operator+(const Position pos) const { return {x_ + pos.x(), y_ + pos.y(), z_ + pos.z()}; }
	Position operator+=(const Position pos) { *this = *this + pos; return *this; }
	Position operator-(const Position pos) const { return {x_ - pos.x(), y_ - pos.y(), z_ - pos.z()}; }
	Position operator-=(const Position pos) { *this = *this - pos; return *this; }
	Position operator*(const float scalar) const { return {x_ * scalar, y_ * scalar, z_ * scalar}; }
	Position operator/(const float scalar) const { return {x_ / scalar, y_ / scalar, z_ / scalar}; }

	[[nodiscard]] auto toVec3() const -> glm::vec3 { return {x_, y_, z_}; };
	[[nodiscard]] auto toString() const -> std::string
	{
		return "x: " + std::to_string(x_) + "; y: " + std::to_string(y_) + "; z: " + std::to_string(z_) + ";";
	}
	[[nodiscard]] auto norm() const { return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2)); }
	[[nodiscard]] auto dist(const Position pos) const -> double
	{
		return sqrt(pow(x_ - pos.x(), 2) + pow(y_ - pos.y(), 2) + pow(z_ - pos.z(), 2));
	}

	static auto dist(const Position pos1, const Position pos2) -> double
	{
		return sqrt(pow(pos1.x() - pos2.x(), 2) + pow(pos1.y() - pos2.y(), 2) + pow(pos1.z() - pos2.z(), 2));
	}

	[[nodiscard]] Position normalize() const
	{
		// Consider the position a vector and normalize it
		const auto value = std::sqrt(x_*x_ + y_*y_ + z_*z_);
		return	{x_ / value, y_ / value, z_ / value};
	}

	[[nodiscard]] float dot(Position pos) const;
	[[nodiscard]] Position cross(Position pos) const;
	[[nodiscard]] float x() const { return x_; }
	[[nodiscard]] float y() const { return y_; }
	[[nodiscard]] float z() const;

private:
	float x_, y_, z_;
};


#endif // POSITION_HPP
