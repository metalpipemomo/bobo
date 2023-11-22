#pragma once

#include "bpch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

class ModelLoader
{
public:
	static void Init()
	{
		GetInstance()->LoadAllModels("../assets/Models");
	}

	static Model* GetModel(const std::string& identifier);

private:
	static ModelLoader* GetInstance()
	{
		static ModelLoader* instance = new ModelLoader();
		return instance;
	}
	ModelLoader() {}
	void LoadModel(const std::string& identifier, const std::string& path);
	void LoadAllModels(const std::string& directory);
	std::unordered_map<std::string, Model*> m_LoadedModels;
    std::mutex m_ModelMtx;
};

Model* ModelLoader::GetModel(const std::string& identifier)
{
	// Copy and make the Identifier all lowercase first
	std::string copyIdentifier = identifier;
	std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);

	auto ml = GetInstance();

    // if the passed in identifier is contained in the map, return the associated Model
    auto it = ml->m_LoadedModels.find(copyIdentifier);
    if (it != ml->m_LoadedModels.end())
    {
        return it->second;
    }

	// not found
	BOBO_WARN("Failed to load Model with Identifier: {} - Ensure the requested asset has been placed into the assets/Models directory", copyIdentifier);
	return nullptr;
}

void ModelLoader::LoadModel(const std::string& identifier, const std::string& path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, GLuint> uniqueVertices{};
    Model* model = new Model();

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.position =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<GLuint>(model->vertices.size());
                model->vertices.push_back(vertex);
            }

            model->indices.push_back(uniqueVertices[vertex]);
        }
    }

	GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(Vertex), model->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices.size() * sizeof(GLuint), model->indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	model->vao = vao;

    std::string copyIdentifier = identifier;
	std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);

    BOBO_INFO("MODEL IS LOADING");

    {
        std::lock_guard<std::mutex> lock(m_ModelMtx);
        BOBO_INFO("MODEL IS INSERTING");
        m_LoadedModels.insert({ copyIdentifier, model });
    }
}

void ModelLoader::LoadAllModels(const std::string& directory)
{
	// Call Load Model for each File inside of Directory
	// The identifier will be the name of the .obj File without the File Type
	// Example: file name = "cube.obj", the identifier will be cube

    std::vector<std::thread> modelThreads;

	for (auto const& dirEntry : std::filesystem::directory_iterator{ directory })
	{
		if (dirEntry.path().extension() != ".obj") continue;
        modelThreads.emplace_back(&ModelLoader::LoadModel, this, dirEntry.path().stem().generic_string(), dirEntry.path().generic_string());
	}

    for (size_t i = 0; i < modelThreads.size(); i++)
    {
        modelThreads[i].join();
    }

	BOBO_INFO("ModelLoader Loaded Models from Directory {}", directory);
}
