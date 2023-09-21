#include "FragmentShader.h"

FragmentShader::FragmentShader() {
	fragdepth = vec4(0, 0, 0, 0);
	fragcolor = vec4(0, 0, 0, 0);
}

FragmentShader::FragmentShader(vec4 dep, vec4 col){
	fragdepth = dep;
	fragcolor = col;
}
