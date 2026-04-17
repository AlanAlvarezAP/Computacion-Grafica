#pragma once

#include "config.h"

const char* vertex_shader="#version 330 core\n"
"layout (location=0) in vec3 inVec;\n"
"uniform mat4 model;\n"
"void main(){\n"
"	gl_Position=model*vec4(inVec.xyz,1.0f);\n"	
"}\n";

const char* fragment_shader="#version 330 core\n"
"uniform vec3 ouCol;\n"
"out vec4 outColor;\n"
"void main(){\n"
"	outColor=vec4(ouCol.rgb,1.0f);\n"
"}\n";

class Matrix;

class Shaders:{
private:
	unsigned int program_ID;
	GLuint vertex_shad_id,fragment_shad_id,program_id;
public:
	Shaders();
	void use();
	unsigned int GetProgram();
	void SetColor(const float &r,const float &g,const float &b);
	void SetMatrix(const Matrix& mat);
	
};
