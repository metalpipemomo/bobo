#include "ShaderLoader.h"

ShaderLoader::ShaderLoader() {
	
	shaderfile = "";

}

/* Loads a shader file string from a file at a particular location.
	@param filelocation
	*/

void ShaderLoader::LoadShader(std::string filelocation) {
	std::string final_string = "";
	std::string line;
	std::ifstream myfile(filelocation);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			final_string += line + '\n';
		}
		myfile.close();
	}
	else {
		std::cout << "File Cannot Be Opened" << std::endl;
	}

	shaderfile = final_string;
};