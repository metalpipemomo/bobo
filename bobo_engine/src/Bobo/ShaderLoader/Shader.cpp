#include "Shader.h"

/* Basic shader constructor, makes a default shader
that has only 0 values in all its' attributes.
There is no shaderfile string since it was not created
using a file.
*/

Shader::Shader() {
	shaderfile = "";
	vs = VertexShader();
	fs = FragmentShader();
}

Shader::Shader(VertexShader v, FragmentShader f, String fl) {
	LoadShader(fl);
	vs = v;
	fs = f;
}

/** Using all the attributes loaded into the shader (vertex shader, etc.),
these changes are updated on the screen to be viewed by the viewer.
*/

void Shader::UpdateShader() {

}

/* Loads a shader file string from a file at a particular location.
	@param filelocation
	*/

void Shader::LoadShader(String filelocation) {
	string final_string = "";
	string line;
	ifstream myfile(filelocation);
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

/** Loads shader attributes through its' parameters (does not use file).
	@param v
	@param f
*/

void Shader::LoadShader(VertexShader v, FragmentShader f) {
	vs = v;
	fs = f;
};