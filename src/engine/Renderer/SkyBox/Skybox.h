#pragma once

#include "../Shaders/SkyBox/SkyBoxShader.h"
#include "../libs/stb/stb_image.h"
#include <gl/gl.h>

/**REFERENCES:
	https://learnopengl.com/Advanced-OpenGL/Cubemaps
	*/

/**
* This Skybox is designed to serve as the game environment.
* Skybox textures will be created and used within this skybox.
*/

class SkyBox {
	public:
		SkyBox(std::vector<std::string> faces);
	private:
		unsigned int LoadCubeMap(std::vector<std::string> faces);
		unsigned int m_cubeMapTexture;
		GLuint m_skyboxVertices;
		SkyBoxShader m_skyboxShader;
};

#include "Skybox.cpp"