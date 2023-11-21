#pragma once

#include "../../bpch.h"

#include "engine/EntityComponent/SceneManager.h"
#include "ShaderLoader.h"
#include "engine/EntityComponent/BaseComponents/Transform.h"
#include "engine/EntityComponent/BaseComponents/Material.h"
#include "Camera.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	static void Init()
	{
		auto r = GetInstance();
		r->SetUniform("model");
		r->SetUniform("view");
		r->SetUniform("projection");
		r->SetUniform("lightColor");
		r->SetUniform("lightPosition");
		r->SetUniform("cameraPosition");
		r->SetUniform("tex");

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

		glUseProgram(ShaderLoader::GetProgram());

		for (auto& material : materials)
		{
			glUniformMatrix4fv(r->m_UniformLocations["model"], 1, 0, glm::value_ptr(material->model));
			glUniformMatrix4fv(r->m_UniformLocations["view"], 1, 0, glm::value_ptr(Camera::GetViewMatrix()));
			glUniformMatrix4fv(r->m_UniformLocations["projection"], 1, 0, glm::value_ptr(Camera::GetProjectionMatrix()));
			glUniform3f(r->m_UniformLocations["lightColor"], 1.0f, 1.0f, 1.0f);
			glUniform3f(r->m_UniformLocations["lightPosition"], 0.0f, 0.0f, -10.0f);
			glUniform3f(r->m_UniformLocations["cameraPosition"], cPos.x, cPos.y, cPos.z);
			glUniform1i(r->m_UniformLocations["tex"], material->texture);

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

	void SetUniform(const std::string& uniformName)
	{
		auto location = glGetUniformLocation(ShaderLoader::GetProgram(), uniformName.c_str());
		m_UniformLocations.insert({ uniformName, location });
	}

	std::map<std::string, GLint> m_UniformLocations;
};