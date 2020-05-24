#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include <glm/glm.hpp>

#include <Renderer.h>


class GameObject2D
{
public:
	glm::vec3 Position;
	glm::vec4 Color;

	GameObject2D(glm::vec3 pos, glm::vec4 color);

	void Draw(Renderer &renderer);
	
};