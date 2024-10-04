#include "object.hpp"

Object::Object(const std::string &modelPath) :
	pos({0, 0, 0}), model(new Model(modelPath)), box(new CollisionBox())
{
}

Object::Object(const Position pos, const std::string &modelPath, const float width, const float height,
			   const float depth, CollisionBox *box) :
	pos(pos), model(new Model(modelPath)), width(width), height(height), depth(depth), box(box)
{
}

Object::~Object()
{
	pos = {0, 0, 0};
	width = 0;
	height = 0;
	depth = 0;
	delete box;
}
void Object::draw(const Shader &shader, const float deltaTime) const
{
	shader.use();
	auto m_model = glm::mat4(1.0f);
	m_model = translate(m_model, pos.toVec3());
	m_model = scale(m_model, glm::vec3(width, height, depth));
	shader.setMat4("model", m_model);
	model->Draw(shader);
}
