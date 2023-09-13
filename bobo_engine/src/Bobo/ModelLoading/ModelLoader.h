#pragma once

#pragma once

#include "bobopch.h"
#include "Model.h"

namespace Bobo
{
	class BOBO_API ModelLoader
	{
	public:
		static ModelLoader& GetInstance() 
		{
			static ModelLoader* instance = new ModelLoader();
			return *instance;
		}
		virtual ~ModelLoader();
		// Will load a Model from a specified file
		int LoadNewModel(std::string& identifier, const std::string& loadFrom);
		// Will load all Models from a specified directory - will ONLY read .obj files
		int LoadAllModels(const std::string& directory);
		// Identifiers will all be converted to lowercase prior to adding & getting, so they are functionally case-insensitive
		Model* GetModel(std::string& identifier);
	private:
		ModelLoader();
		std::unordered_map<std::string, Model*> m_LoadedModels;
	};
}