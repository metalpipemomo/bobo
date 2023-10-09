#include "bpch.h"

#include "ModelLoader.h"

struct VertexData
{
    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLfloat> colors;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) : position(pos), normal(norm), texCoord(tex) {}
};

int ModelLoader::LoadNewModel(const std::string& identifier, const std::string& path)
{
    std::ifstream file{ path };

    std::vector<Vertex> verts{};

    std::vector<glm::vec3> positions{};
    std::vector<glm::vec2> texCoords{};
    std::vector<glm::vec3> normals{};

    std::vector<float> finalPositions;
    std::vector<float> finalTexCoords;
    std::vector<float> finalNormals;

    std::vector<int> indices{};

    std::string currentLine;
    while (std::getline(file, currentLine, '\n'))
    {
        std::istringstream iss(currentLine);
        std::string marker;
        iss >> marker;
        if (marker == "v")
        {
            float x;
            float y;
            float z;
            iss >> x;
            iss >> y;
            iss >> z;
            positions.emplace_back(glm::vec3{ x, y, z });
        } else if (marker == "vt")
        {
            float x;
            float y;
            iss >> x;
            iss >> y;
            texCoords.emplace_back(glm::vec2{ x, y });
        } else if (marker == "vn")
        {
            float x;
            float y;
            float z;
            iss >> x;
            iss >> y;
            iss >> z;
            normals.emplace_back(glm::vec3{ x, y, z });
        } else if (marker == "f")
        {
            std::string tri1;
            iss >> tri1;
            std::stringstream elem1(tri1);

            std::string tri2;
            iss >> tri2;
            std::stringstream elem2(tri2);

            std::string tri3;
            iss >> tri3;
            std::stringstream elem3(tri3);

            //Waveform format is 1-indexed
            std::string v1s;
            std::getline(elem1, v1s, '/');
            int v1 = std::stoi(v1s) - 1;
            verts.emplace_back(Vertex(positions.at(v1), glm::vec3{ 0,0,0 }, glm::vec2{ 0, 0 }));
            std::getline(elem1, v1s, '/');
            verts.at(verts.size() - 1).texCoord = texCoords.at(std::stoi(v1s) - 1);
            std::getline(elem1, v1s, '/');
            verts.at(verts.size() - 1).normal = normals.at(std::stoi(v1s) - 1);
            indices.push_back(static_cast<int>(verts.size() - 1));

            std::string v2s;
            std::getline(elem2, v2s, '/');
            int v2 = std::stoi(v2s) - 1;
            verts.emplace_back(Vertex(positions.at(v2), glm::vec3{ 0,0,0 }, glm::vec2{ 0, 0 }));
            std::getline(elem2, v2s, '/');
            verts.at(verts.size() - 1).texCoord = texCoords.at(std::stoi(v2s) - 1);
            std::getline(elem2, v2s, '/');
            verts.at(verts.size() - 1).normal = normals.at(std::stoi(v2s) - 1);
            indices.push_back(static_cast<int>(verts.size() - 1));

            std::string v3s;
            std::getline(elem3, v3s, '/');
            int v3 = std::stoi(v3s) - 1;
            verts.emplace_back(Vertex(positions.at(v3), glm::vec3{ 0,0,0 }, glm::vec2{ 0, 0 }));
            std::getline(elem3, v3s, '/');
            verts.at(verts.size() - 1).texCoord = texCoords.at(std::stoi(v3s) - 1);
            std::getline(elem3, v3s, '/');
            verts.at(verts.size() - 1).normal = normals.at(std::stoi(v3s) - 1);
            indices.push_back(static_cast<int>(verts.size() - 1));
        }
    }
    for (int i = 0; i < verts.size(); ++i)
    {
        finalPositions.push_back(verts[i].position.x);
        finalPositions.push_back(verts[i].position.y);
        finalPositions.push_back(verts[i].position.z);
        finalNormals.push_back(verts[i].normal.x);
        finalNormals.push_back(verts[i].normal.y);
        finalNormals.push_back(verts[i].normal.z);
        finalTexCoords.push_back(verts[i].texCoord.x);
        finalTexCoords.push_back(verts[i].texCoord.y);
    }

	// Make Model from Data
	Model* newModel = new Model(finalPositions, finalTexCoords, finalNormals, indices);

	// Add this newly loaded Model to Map of Known Models
	// Make the Identifier all Lowercase after copying first
	std::string copyIdentifier = identifier;
	std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::tolower);
	m_LoadedModels.insert(std::pair<std::string, Model*>(copyIdentifier, newModel));

	// Log some Info
	BOBO_INFO("Model Loaded From: " + path + " - Identifier: " + copyIdentifier);

	return 0;
}

int ModelLoader::LoadAllModels(const std::string& directory)
{
	// Call Load Model for each File inside of Directory
	// The identifier will be the name of the .obj File without the File Type
	// Example: file name = "cube.obj", the identifier will be cube
	int count = 0;
	for (auto const& dirEntry : std::filesystem::directory_iterator{ directory })
	{
		if (dirEntry.path().extension() != ".obj") continue;
		LoadNewModel(dirEntry.path().stem().generic_string(), dirEntry.path().generic_string());
		count++;
	}

	BOBO_INFO("ModelLoader Loaded {} Models from Directory {}", count, directory);

	return count;
}

Model* ModelLoader::GetModel(const std::string& identifier)
{
	// Copy and make the Identifier all lowercase first
	std::string copyIdentifier = identifier;
	std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::tolower);

	auto ml = GetInstance();

	// if the passed in identifier is contained in the map, return the associated Model
	if (ml->m_LoadedModels.count(copyIdentifier) == 1)
	{
		// Found
		BOBO_INFO("Found Model with Identifier {}", copyIdentifier);
		return ml->m_LoadedModels.at(copyIdentifier);
	} else
	{
		// not found
		BOBO_WARN("No Model with Identifier {} has been Loaded", copyIdentifier);
		return nullptr;
	}
}