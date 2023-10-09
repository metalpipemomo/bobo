#pragma once

#include "bpch.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "engine/Renderer/ShaderLoader.h"

struct Model
{
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> normals;
	std::vector<int> indices;
	GLuint vao;

	Model(std::vector<float> verts, std::vector<float> tcs, std::vector<float> norms, std::vector<int> idx)
		: vertices(verts), texCoords(tcs), normals(norms), indices(idx)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		auto loc = glGetAttribLocation(ShaderLoader::GetProgram(), "position");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(loc);

		GLuint normalBuffer;
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
		loc = glGetAttribLocation(ShaderLoader::GetProgram(), "normal");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(loc);

		GLuint texCoordBuffer;
		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);
		loc = glGetAttribLocation(ShaderLoader::GetProgram(), "texCoord");
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(loc);

		GLuint indexBuffer;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
};