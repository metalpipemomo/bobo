#include "bobopch.h"

#include "ModelLoader.h"

namespace Bobo
{
	int ModelLoader::LoadNewModel(std::string& identifier, const std::string& loadFrom)
	{
		// Initialize Vectors to store each of the Following while reading: Vertices, UVs, Normals 
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempUVs;
		std::vector<glm::vec3> tempNormals;

		// Initialize Vectors which will store our final results
		std::vector<glm::vec3> finalVertices;
		std::vector<glm::vec2> finalUVs;
		std::vector<glm::vec3> finalNormals;

		// Turn String -> Data (Array of Vertices, UVs, Normals)
		FILE* file = fopen(loadFrom.c_str(), "r");
		if (file == NULL) {
			BOBO_ERROR("The file specified could not be loaded: " + loadFrom);
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
				// Scan floats into a new Vector3
				glm::vec3 vertex;
				if (fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) != 3)
				{
					// Error
					BOBO_ERROR("Could not read Vertex");
					return 1;
				}
				// Add Vector3 to Vertices Array
				tempVertices.push_back(vertex);
			}
			else if (strcmp(token, "vt") == 0)
			{
				// Line starts with vt -> Line represents a Texture Coordinate of a Vertex, Line contains 2 floats
				// Scan floats into a new Vector2
				glm::vec2 uv;
				if (fscanf(file, "%f %f\n", &uv.x, &uv.y) != 2) 
				{
					// Error
					BOBO_ERROR("Could not read UV");
					return 1;
				}
				// Add Vector2 to Texture Coordinates Array
				tempUVs.push_back(uv);
			}
			else if (strcmp(token, "vn") == 0)
			{
				// Line starts with vn -> Line represents the Normal of a Vertex, Line contains 3 floats
				// Scan floats into a new Vector3
				glm::vec3 normal;
				if (fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z) != 3) 
				{
					// Error
					BOBO_ERROR("Could not read Normal");
					return 1;
				}
				// Add Vector3 to Vertex Normals Array
				tempNormals.push_back(normal);
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
				unsigned int vertexIndex[3], uvIndex[3], normalsIndex[3];

				// Read Info Combonations (ex: 5/11/2) into these Arrays respectively
				if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalsIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalsIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalsIndex[2]) != 9) 
				{
					// Handle Face cannot be Read Error
					BOBO_ERROR("Could not read Face");

					return 1;
				}

				// Deal with Vertices
				finalVertices.push_back(tempVertices[vertexIndex[0] - 1]);
				finalVertices.push_back(tempVertices[vertexIndex[1] - 1]);
				finalVertices.push_back(tempVertices[vertexIndex[2] - 1]);

				// Deal with UVs
				finalUVs.push_back(tempUVs[uvIndex[0] - 1]);
				finalUVs.push_back(tempUVs[uvIndex[1] - 1]);
				finalUVs.push_back(tempUVs[uvIndex[2] - 1]);

				// Deal with Normals
				finalNormals.push_back(tempNormals[normalsIndex[0] - 1]);
				finalNormals.push_back(tempNormals[normalsIndex[1] - 1]);
				finalNormals.push_back(tempNormals[normalsIndex[2] - 1]);
			}
		}

		// Close File
		fclose(file);

		// Make Model from Data
		Model* newModel = new Model(finalVertices, finalUVs, finalNormals);

		// Add this newly loaded Model to Map of Known Models
		// Make the Identifier all Lowercase first
		std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
		m_LoadedModels.insert(std::pair<std::string, Model*>(identifier, newModel));

		// Log some Info
		BOBO_INFO("Model Loaded From: " + loadFrom + " - Identifier: " + identifier);

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

	Model* ModelLoader::GetModel(std::string& identifier) 
	{
		// Make the Identifier all lowercase first
		std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);

		// if the passed in identifier is contained in the map, return the associated Model
		if (m_LoadedModels.count(identifier) == 1) 
		{
			// Found
			BOBO_INFO("Found Model with Identifier {}", identifier);
			return m_LoadedModels.at(identifier);
		}
		else 
		{
			// not found
			BOBO_WARN("No Model with Identifier {} has been Loaded", identifier);
			return nullptr;
		}
	}
}