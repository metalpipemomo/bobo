#pragma once

#include "bobopch.h"

/** Used to the render the geometry of the object/scene.
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