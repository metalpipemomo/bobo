#pragma once

#include "../../bpch.h"

#include "engine/EntityComponent/SceneManager.h"
#include "engine/EntityComponent/BaseComponents/Transform.h"
#include "engine/EntityComponent/BaseComponents/Material.h"
#include "Camera.h"
#include "Shader.h"
#include "Shaders/Standard/StandardShader.h"
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
		auto r = GetInstance();
		auto cPos = Camera::GetPosition();

		r->p_StandardShader->Use();

		for (auto& material : materials)
		{
			StandardShaderProps ssp;
			ssp.model =  glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ssp.projection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 40.0f);
			ssp.view = glm::lookAt(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ssp.texture = material->texture;
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