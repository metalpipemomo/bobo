#include "VertexShader.h"

VertexShader::VertexShader() {
	position = vec4(0, 0, 0, 0);
	color = vec4(0, 0, 0, 0);
}

VertexShader::VertexShader(vec4 pos, vec4 col) {
	position = pos;
	color = col;
}