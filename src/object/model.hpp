#ifndef MODEL_HPP
#define MODEL_HPP

#include <utils/position.hpp>

class Model
{
public:
	explicit Model(const char *path, unsigned int texture);
	~Model();
	void draw(Position *pos, float rotationAngle, unsigned int shaderProgram) const;

private:
	unsigned int VAO{}, VBO{}, EBO{};
	unsigned int indicesCount{};
	unsigned int texture{};
};

#endif // MODEL_HPP
