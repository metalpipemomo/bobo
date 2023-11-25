#pragma once

#include "../../bpch.h" //Gets all the bobo library items


/**
* This Skybox is designed to serve as the game environment.
* Skybox textures will be created and used within this skybox.
*/

class SkyBox {
	public:
		SkyBox(vector<std::string> faces);
	private:
		unsigned int LoadCubeMap(vector<std::string> faces);
		unsigned int m_cubeMapTexture;
};