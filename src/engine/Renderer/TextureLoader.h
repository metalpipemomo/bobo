#pragma once

#include "bpch.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class TextureLoader
{
public:
	static void Init()
	{
		auto tl = GetInstance();
		tl->LoadAllTextures("src/game/Textures");
	}

	static int GetTexture(const std::string& identifier)
	{
		auto tl = GetInstance();
		auto it = tl->m_TexMap.find(identifier);
		if (it != tl->m_TexMap.end())
		{
			return it->second;
		}

		BOBO_WARN("Failed to Load Texture with Identifer: {}", identifier);
		return -1;
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
			if (dirEntry.path().extension() != ".jpg") continue;
			LoadTexture(dirEntry.path().generic_string(), dirEntry.path().stem().generic_string());
			count++;
		}
		BOBO_INFO("Texture Loader loaded {} textures from Directory {}", count, directory);

		return count;
	}
	
	void LoadTexture(const std::string& path, const std::string& identifier)
	{
		int width, height, numChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

		BOBO_ASSERT(data, "Failed to load texture...");

		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + m_TexCount++);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		
		std::string copyIdentifier = identifier;
		std::transform(copyIdentifier.begin(), copyIdentifier.end(), copyIdentifier.begin(), ::tolower);
		m_TexMap.insert({ copyIdentifier, texture });
	}

	int m_TexCount;
	std::map<std::string, unsigned int> m_TexMap;
};