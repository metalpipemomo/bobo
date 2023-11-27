#pragma once

#include "../../bpch.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class TextureLoader
{
public:
	static void Init()
	{
		auto tl = GetInstance();
		tl->LoadAllTextures("../assets/Textures");
	}

	static unsigned int GetTexture(const std::string& identifier)
	{
		std::string copyIdentifier = identifier;
		std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);

		auto tl = GetInstance();

		auto it = tl->m_TexMap.find(copyIdentifier);
		if (it != tl->m_TexMap.end())
		{
			return it->second;
		}

		BOBO_WARN("Failed to load Texture with Identifer: {} - Ensure the requested asset has been placed into the assets/Textures directory and is a .png", 
			copyIdentifier);
		return -1;
	}

	static void IncrementCount()
	{
		auto tl = GetInstance();
		tl->m_TexCount++;
	}

	static int GetCount()
	{
		return GetInstance()->m_TexCount;
	}

private:
	static TextureLoader* GetInstance()
	{
		static TextureLoader* instance = new TextureLoader();
		return instance;
	}

	TextureLoader()
	{
		m_TexCount = 0;
	}

	int LoadAllTextures(const std::string& directory)
	{
		int count = 0;
		for (const auto& dirEntry : std::filesystem::directory_iterator{ directory })
		{
			LoadTexture(dirEntry.path().generic_string(), dirEntry.path().stem().generic_string());
			count++;
		}
		BOBO_INFO("Texture Loader loaded {} textures from Directory {}", count, directory);

		return count;
	}
	
	void LoadTexture(const std::string& path, const std::string& identifier)
	{
		int width, height, numChannels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);

		BOBO_ASSERT(data, "Failed to load texture...");

		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + m_TexCount);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (GLAD_GL_ARB_texture_filter_anisotropic)
    	{
        	GLfloat maxAnisotropy = 0.0f;
        	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
    	}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		
		std::string copyIdentifier = identifier;
		std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::toupper);
		m_TexMap.insert({ copyIdentifier, m_TexCount++ });
	}

	int m_TexCount;
	std::map<std::string, unsigned int> m_TexMap;
};