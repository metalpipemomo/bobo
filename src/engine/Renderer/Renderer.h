#pragma once

#include "../../bpch.h"

#include "engine/EntityComponent/SceneManager.h"
#include "engine/EntityComponent/BaseComponents/Transform.h"
#include "engine/EntityComponent/BaseComponents/Material.h"
#include "engine/EntityComponent/BaseComponents/SpotlightComponent.h"
#include "engine/EntityComponent/BaseComponents/PointlightComponent.h"
#include "Camera.h"
#include "Shader.h"
#include "Shaders/Standard/StandardShader.h"
#include "Light.h"
#include <glad/glad.h>

class Renderer
{
public:
	static void Init()
	{
		auto r = GetInstance();

		r->p_StandardShader = new StandardShader();

		BOBO_INFO("Renderer initialized!");
	}

	static void Update()
	{
		auto scene = SceneManager::GetActiveScene();
		auto materials = scene->GetComponentsOfType<Material>();

		for (auto& material : materials)
		{
			auto transform = scene->GetComponent<Transform>(material->m_OwnerId);

			glm::mat4 translation = glm::identity<glm::mat4>();
			translation = glm::translate(translation, transform->position);
			glm::mat4 rotation = glm::identity<glm::mat4>();
			rotation = glm::rotate(rotation, transform->rotation.x, { 1, 0, 0 });
			rotation = glm::rotate(rotation, transform->rotation.y, { 0, 1, 0 });
			rotation = glm::rotate(rotation, transform->rotation.z, { 0, 0, 1 });
			glm::mat4 scale = glm::identity<glm::mat4>();
			scale = glm::scale(scale, transform->scale);
			material->model = translation * rotation * scale;
		}
	}

	static void Draw()
	{
		auto scene = SceneManager::GetActiveScene();
		auto materials = scene->GetComponentsOfType<Material>();
		auto spotlights = scene->GetComponentsOfType<SpotlightComponent>();
		auto pointlights = scene->GetComponentsOfType<PointlightComponent>();
		auto r = GetInstance();
		auto cPos = Camera::GetPosition();

		r->p_StandardShader->Use();

		for (auto& material : materials)
		{
			StandardShaderProps ssp;
			ssp.model = material->model;
			ssp.projection = Camera::GetProjectionMatrix();
			ssp.view = Camera::GetViewMatrix();
			ssp.texture = material->texture;
			ssp.shininess = 32.0f;
			ssp.cameraPos = Camera::GetPosition();
			ssp.spotlights = spotlights;
			ssp.pointlights = pointlights;
			r->p_StandardShader->Data(ssp);

			glBindVertexArray(material->modelData->vao);
			
			glDrawElements(GL_TRIANGLES, (GLsizei)material->modelData->indices.size(), GL_UNSIGNED_INT, 0);
			
			glBindVertexArray(0);
		}
	}

private:
	static Renderer* GetInstance()
	{
		static Renderer* instance = new Renderer();
		return instance;
	}

	Renderer() {}
	StandardShader* p_StandardShader;
};