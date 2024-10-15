#include "object.hpp"

void Object::draw(const Shader &shader, const float deltaTime) const
{
	auto m_model = glm::mat4(1.0f);
	m_model = translate(m_model, pos.toVec3());
	m_model = scale(m_model, glm::vec3(width, height, depth));
	shader.setMat4("model", m_model);
	model->draw(shader);
}
