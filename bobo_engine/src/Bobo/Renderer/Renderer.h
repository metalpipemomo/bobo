#pragma once

namespace bobo
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();
		// eventually we want to be taking from an entity obj but right now lets take a model and render it
		DrawVertices();
		DrawNormals();
		
	private:
		
	};
}