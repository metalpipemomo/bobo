#pragma once

#include "bobopch.h"

/** Used to render the colour of the object/scene.
*/

namespace Bobo
{
	class FragmentShader
	{
	public:
		FragmentShader();
		FragmentShader(vec4 dep, vec4 col);
		virtual ~FragmentShader() {
			delete fragdepth;
			delete fragcolor;
		};
	private:
		vec4 fragdepth;
		vec4 fragcolor;
	};
}