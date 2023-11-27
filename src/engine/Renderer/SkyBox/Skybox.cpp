#include "Skybox.h"

/**REFERENCES:
    https://learnopengl.com/Advanced-OpenGL/Cubemaps
    */

//Public Functions

/** Skybox constructor, which takes in an array of texture file directories
to load up a cube map tailored to those textures. Then, that cube map serves as
the sky box's cubeMapTexture. 

@param faces
*/

SkyBox::Skybox(vector<std::string> faces) {
    m_cubeMapTexture = LoadCubeMap(faces);
    m_skyboxShader = new SkyBoxShader();

	glDepthMask(GL_FALSE);
	skyboxShader.use();
	// ... set view and projection matrix
	glBindVertexArray(m_skyboxVertices);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
};

//Private Functions

/** Takes in an array of texture file directories, and applies those
textures to a cubemap. These are then wrapped and filtered and returned
as an unsigned integer.

@param faces
@return textureID
*/

SkyBox::unsigned int LoadCubeMap(vector<std::string> faces) {
	//Creates Cubemap Texture ID
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//Apply Each Texture for Each Face of the Cube Map
	int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    //Wrapping and filtering methods are determined here.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
};