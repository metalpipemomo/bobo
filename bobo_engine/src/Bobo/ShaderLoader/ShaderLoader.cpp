#include "ShaderLoader.h"

ShaderLoader::ShaderLoader() {
	
	shaderfile = "";

}

/* Loads a shader file string from a file at a particular location.
	@param filelocation
	*/

std::string ShaderLoader::LoadShader(std::string filelocation) {
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

	return final_string;
};

void ShaderLoader::ParseShader(std::string filelocation) {
	std::string data = LoadShader(filelocation);
	//Parse this the data from the file, then input it into the loaded shaders map.
};