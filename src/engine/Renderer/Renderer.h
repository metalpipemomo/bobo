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
#include "Shaders/SkyBox/SkyBoxShader.h"
#include "../Model/ModelLoader.h"
#include "TextureLoader.h"
#include <glad/glad.h>

// Hard-coded vertices for skybox cube
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

class Renderer
{
public:
	// Initializes shaders and generates the VAO for the skybox
	static void Init()
	{
		auto r = GetInstance();

		r->p_StandardShader = new StandardShader();
		r->p_SkyBoxShader = new SkyBoxShader();

		r->p_SkyBoxShader->Use();

		GLuint skyboxVBO;
		glGenVertexArrays(1, &r->skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(r->skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		BOBO_INFO("Renderer initialized!");
	}

	// Updates the model matrix of each entity with a transform every frame
	static void Update()
	{
		auto scene = SceneManager::GetActiveScene();
		auto materials = scene->GetComponentsOfType<Material>();

		for (auto& material : materials)
		{
			auto transform = scene->GetComponent<Transform>(material->m_OwnerId);

			glm::mat4 translation = glm::identity<glm::mat4>();
			translation = glm::translate(translation, transform->position);
			glm::mat4 rotX = glm::rotate(glm::identity<glm::mat4>(), transform->rotation.x, { 1, 0, 0 });
			glm::mat4 rotY = glm::rotate(glm::identity<glm::mat4>(), transform->rotation.y, { 0, 1, 0 });
			glm::mat4 rotZ = glm::rotate(glm::identity<glm::mat4>(), transform->rotation.z, { 0, 0, 1 });


			glm::mat4 rotation = rotZ * rotY * rotX;
			
			glm::mat4 scale = glm::identity<glm::mat4>();
			scale = glm::scale(scale, transform->scale);
			material->model = translation * rotation * scale;
		}
	}

	// Draws standard geometry
	// Then changes depth function to draw skybox behind everything
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
			ssp.texture = 0;
			ssp.shininess = 32.0f;
			ssp.cameraPos = Camera::GetPosition();
			ssp.spotlights = spotlights;
			ssp.pointlights = pointlights;
			r->p_StandardShader->Data(ssp);

			glBindVertexArray(material->modelData->vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->texture);
			
			glDrawElements(GL_TRIANGLES, (GLsizei)material->modelData->indices.size(), GL_UNSIGNED_INT, 0);
			
			glBindVertexArray(0);
		}

		auto skyBoxModel = ModelLoader::GetModel("cube");
		auto skyBoxTexture = TextureLoader::GetTexture("SKYBOX");

		glDepthFunc(GL_LEQUAL);
		r->p_SkyBoxShader->Use();

		SkyBoxShaderProps skyProps;
		skyProps.view = glm::mat4(glm::mat3(Camera::GetViewMatrix()));
		skyProps.projection = Camera::GetProjectionMatrix();
		skyProps.texture = 0;
		r->p_SkyBoxShader->Data(skyProps);
		
		glBindVertexArray(r->skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

private:

	static Renderer* GetInstance()
	{
		static Renderer* instance = new Renderer();
		return instance;
	}

	Renderer() {}
	StandardShader* p_StandardShader;
	SkyBoxShader* p_SkyBoxShader;
	GLuint skyboxVAO;
};
