#pragma once

#include "Model.h"
#include "glm/glm.hpp"

class ModelLoader
{
public:
	static void Init()
	{
		GetInstance()->LoadAllModels("src/game/Models");
	}

	static Model* GetModel(const std::string& identifier);

private:
	static ModelLoader* GetInstance()
	{
		static ModelLoader* instance = new ModelLoader();
		return instance;
	}
	ModelLoader() {}
	int LoadNewModel(const std::string& identifier, const std::string& path);
	int LoadAllModels(const std::string& directory);
	std::unordered_map<std::string, Model*> m_LoadedModels;
};