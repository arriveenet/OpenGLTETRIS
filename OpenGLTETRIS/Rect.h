#pragma once

#include <glm/glm.hpp>

class Rect
{
public:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec2 m_vertex[4];
	glm::vec2 m_texCoord[4];

	Rect();
	Rect(glm::vec2 _size);
	Rect(glm::vec2 _size, glm::vec2 _position);

	void setSize(const glm::vec2& _size);
	void setPosition(const glm::vec2& _position);

	void draw();
};

