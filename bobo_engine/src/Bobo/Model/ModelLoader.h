#pragma once

#include "Model.h"
#include <glm/glm.hpp>

namespace Bobo
{

	class BOBO_API ModelLoader
	{
	public:
		static ModelLoader* GetInstance() 
		{
			static ModelLoader instance = ModelLoader();
			return &instance;
		}
		// Will load a Model from a specified file
		int LoadNewModel(std::string& identifier, const std::string& loadFrom);
		// Will load all Models from a specified directory - will ONLY read .obj files
		int LoadAllModels(const std::string& directory);
		// Identifiers will all be converted to lowercase prior to adding & getting, so they are functionally case-insensitive
		Model* GetModel(std::string& identifier);
	private:
		ModelLoader() {}
		std::unordered_map<std::string, Model*> m_LoadedModels;
	};
}