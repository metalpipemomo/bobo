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
	// Initialize Vectors which will store our final results
	std::vector<float> positions;
	std::vector<float> uvs;
	std::vector<float> normals;
	std::vector<int> indices;

	// Turn String -> Data (Array of Vertices, UVs, Normals)
	FILE* file = fopen(path.c_str(), "r");
	if (file == NULL) {
		BOBO_ERROR("The file specified could not be loaded: {} ", path);
		return 1;
	}

	// Sample Data
	// v 1.000000 1.000000 -1.000000 <- Vertex
	// vn -0.0000 1.0000 -0.0000 <- Normal
	// vt 0.625000 0.500000 <- UV
	// f 5/5/1 3/3/1 1/1/1 <- Face

	// Read File
	while (true)
	{
		// Read the next token
		// if it is a header we care about (v, vn, vt, f) we'll go through a certain set of steps depending of which header it is
		char token[32] = { 0 };
		int res = fscanf(file, "%31s", token);

		// Reached the end of the file, break the loop
		if (res == EOF)
			break;

		if (strcmp(token, "v") == 0)
		{
			// Line starts with v -> Line represents a Vertex, Line contains 3 floats
			// Scan floats into variables
			glm::vec3 pos;
			if (fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z) != 3)
			{
				// Error
				BOBO_ERROR("Could not read Vertex");
				return 1;
			}
			BOBO_WARN("Read Vertex: <{}, {}, {}>", pos.x, pos.y, pos.z);

			// Add to Vertices Vector
			positions.insert(positions.end(), { pos.x, pos.y, pos.z });
		}
		else if (strcmp(token, "vt") == 0)
		{
			// Line starts with vt -> Line represents a Texture Coordinate of a Vertex, Line contains 2 floats
			// Scan floats into variables
			glm::vec2 uv;
			if (fscanf(file, "%f %f\n", &uv.x, &uv.y) != 2)
			{
				// Error
				BOBO_ERROR("Could not read UV");
				return 1;
			}
			BOBO_WARN("Read UV: <{}, {}>", uv.x, uv.y);

			// Add to Texture Coordinates Vector
			uvs.insert(uvs.end(), { uv.x, uv.y });
		}
		else if (strcmp(token, "vn") == 0)
		{
			// Line starts with vn -> Line represents the Normal of a Vertex, Line contains 3 floats
			// Scan floats into variables
			glm::vec3 norm;
			if (fscanf(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z) != 3)
			{
				// Error
				BOBO_ERROR("Could not read Normal");
				return 1;
			}
			BOBO_WARN("Read Normal: <{}, {}, {}>", norm.x, norm.y, norm.z);

			// Add to Vertex Normals Vector
			normals.insert(normals.end(), { norm.x, norm.y, norm.z });
		}
		else if (strcmp(token, "f") == 0)
		{
			/*
				Line starts with f -> Line represents a Face, which utilizes all the data above, Line contains 3 of these : X / Y / Z where...
				X: the Index of the Vertex to Use
				Y: the Index of the Texture Coordinate to Use
				Z: the Index of the Normal to Use
			*/
			// Make 3 new Arrays, one for each Piece of Data
			int posIndex[3], uvIndex[3], normalsIndex[3];

			// Read Info Combonations (ex: 5/11/2) into these Arrays respectively
			if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&posIndex[0], &uvIndex[0], &normalsIndex[0],
				&posIndex[1], &uvIndex[1], &normalsIndex[1],
				&posIndex[2], &uvIndex[2], &normalsIndex[2]) != 9)
			{
				// Handle Face cannot be Read Error
				BOBO_ERROR("Could not read Face");

				return 1;
			}

			BOBO_WARN("Read Vertex Pos from Face: <{}, {}, {}>", posIndex[0], posIndex[1], posIndex[2]);

			// Deal with Vertices
			indices.insert(indices.end(), { posIndex[0] - 1, posIndex[1] - 1, posIndex[2] - 1 });
		}
	}

	// Close File
	fclose(file);

	// Make Model from Data
	Model* newModel = new Model(positions, uvs, normals, indices);

	// Add this newly loaded Model to Map of Known Models
	// Make the Identifier all Lowercase first
	std::string copyIdentifier = identifier;
	std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::tolower);
	m_LoadedModels.insert(std::pair<std::string, Model*>(copyIdentifier, newModel));

	// Log some Info
	BOBO_INFO("Model Loaded From: {} - Identifier: {}", path, identifier);

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