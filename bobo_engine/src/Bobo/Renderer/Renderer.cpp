#include "bobopch.h";
#include "Renderer.h";

namespace bobo
{
	Renderer::Renderer()
	{
		// list of models
	}

	Renderer::~Renderer()
	{

	}

	Renderer::DrawVertices()
	{
		// the model/entity should have a list of vertex points, we want to take these and put them into a vertex buffer object.
		// we then bind the buffer
		// afterwards we take that buffer data and draw the vertex points.
		// should be returning the buffer
	}

	Renderer::DrawNormals()
	{
		// same thing as vertices but we are dealing with the normals of the model instead
	}

	// we probably need colours too

}