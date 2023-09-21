#pragma once

#include "bobopch.h"

/** Holds the VertexShader Data of an object
*/

namespace Bobo
{
	class VertexShader
	{
	public:
		VertexShader();
		VertexShader(vec4 pos, vec4 col);
		virtual ~VertexShader() {	
			delete position;
			delete color;
		};
	private:
		vec4 position;
		vec4 color;
	};
}

