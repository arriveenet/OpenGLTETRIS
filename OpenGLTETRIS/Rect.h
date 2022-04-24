#pragma once

#include <glm/glm.hpp>

class Rect
{
public:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec2 m_vertex[4];

	Rect();
	Rect(glm::vec2 _size, glm::vec2 _position);

	void draw();
};

