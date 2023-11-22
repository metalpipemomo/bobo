#pragma once

#include "../Component.h"
#include "Transform.h"
#include "engine/Model/Model.h"
#include <glm/glm.hpp>

class Material : public Component
{
public:
	Model* modelData;
	unsigned int texture;
	glm::mat4 model;

	Material(Model* modelData, unsigned int tex)
		: modelData(modelData), texture(tex)
	{
		model = glm::identity<glm::mat4>();
	}

	Material(Model* modelData, const std::string& name)
		: modelData(modelData)
	{
		model = glm::identity<glm::mat4>();
		texture = TextureLoader::GetTexture(name);
	}

	Material(Model* modelData)
		: modelData(modelData), texture(TextureLoader::GetTexture("white"))
	{
		model = glm::identity<glm::mat4>();
	}
};