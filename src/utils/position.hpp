#ifndef POSITION_HPP
#define POSITION_HPP

#include <cmath>
#include <glm/vec3.hpp>

class Position
{
public:
	Position();
	Position(float x, float y, float z);

	void setPos(float x, float y, float z);

	Position operator+(Position pos) const;
	Position operator-(Position pos) const;
	Position operator*(float scalar) const;
	Position operator/(float scalar) const;

	[[nodiscard]] auto toVec3() const -> glm::vec3 { return {x_, y_, z_}; };
	[[nodiscard]] auto norm() const { return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2)); }
	[[nodiscard]] auto dist(const Position pos) const -> double
	{
		return sqrt(pow(x_ - pos.x(), 2) + pow(y_ - pos.y(), 2) + pow(z_ - pos.z(), 2));
	}

	static auto dist(const Position pos1, const Position pos2) -> double
	{
		return sqrt(pow(pos1.x() - pos2.x(), 2) + pow(pos1.y() - pos2.y(), 2) + pow(pos1.z() - pos2.z(), 2));
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
