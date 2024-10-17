#include "object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

void Object::draw(const Shader &shader, const float deltaTime) const
{
	auto m_model = glm::mat4(1.0f);
	m_model = translate(m_model, pos.toVec3());
	m_model = scale(m_model, glm::vec3(width, height, depth));
	m_model = glm::rotate(m_model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	m_model = glm::rotate(m_model, yaw, glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", m_model);
	model->draw(shader);
}
