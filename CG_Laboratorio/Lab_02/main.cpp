/*
	Pasos basicos para OpenGL
	1. Inicializar GLFW y el perfil a usar
	2. Configurar Ventana para hacer contexto esa ventana
	3. Cargar GLAD
	4. Colocar Callbacks
	5. Bucle principal de rendering 
	5.1 Swapear buffers
	5.2 Buscar eventos
*/

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>
#include <string>

GLuint vertex_shad_id,fragment_shad_id,program_id_cuadrado,program_id_triang,program_id;
GLuint VAO_house,VBO_house,EBO_house,VAO_window,VBO_window,EBO_window,VAO_triang,VBO_triang,EBO_triang;

const char* vertex_shader="#version 330 core\n"
"layout (location=0) in vec3 inVec;\n"
"out vec3 ouCol;\n"
"void main(){\n"
"	gl_Position=vec4(inVec.xyz,1.0f);\n"	
"}\n";

const char* fragment_shader_casa="#version 330 core\n"
"out vec4 outColor;\n"
"void main(){\n"
"	outColor=vec4(0.0f,0.0f,1.0f,1.0f);\n"
"}\n";

const char* fragment_shader_techo="#version 330 core\n"
"out vec4 outColor;\n"
"void main(){\n"
"	outColor=vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\n";

const char* fragment_shader_vert="#version 330 core\n"
"out vec4 outColor;\n"
"void main(){\n"
"	outColor=vec4(0.0f,1.0f,0.0f,1.0f);\n"
"}\n";


float vector_base_cuadrado[]={
	-0.1f,-0.4f,0.0f, // 1 0 0 
	-0.4f,-0.4f,0.0f, // 2 1 1
	-0.4f,0.4f,0.0f, // 3 2 2
	0.4f,0.4f,0.0f, // 7 3 3
	0.4f,-0.4f,0.0f, // 8 4 4
	0.1f,-0.4f,0.0f, // 9 5 5
	0.1f,-0.2f,0.0f, // 10 6 6 
	-0.1f,-0.2f,0.0f, // 11 7 7
	-0.1f,0.0f,0.0f, // 12 0 8
	-0.3f,0.0f,0.0f, // 13 1 9
	-0.3f,0.2f,0.0f, // 14 2 10
	-0.1f,0.2f,0.0f, // 15 3 11
	0.1f,0.0f,0.0f, // 16 4 12
	0.3f,0.0f,0.0f, // 13 5 13
	0.3f,0.2f,0.0f, // 14 6 14
	0.1f,0.2f,0.0f, // 15 7 15
};

float vector_base_triang[]={
	-0.7f,0.4f,0.0f, // 4 0
	0.0f,0.9f,0.0f, // 5 1 
	0.7f,0.4f,0.0f, // 6 2
};


float vector_base[]={
	-0.1f,-0.4f,0.0f, // 1 0
	-0.4f,-0.4f,0.0f, // 2 1 
	-0.4f,0.4f,0.0f, // 3 2
	0.4f,0.4f,0.0f, // 7 3
	0.4f,-0.4f,0.0f, // 8 4
	0.1f,-0.4f,0.0f, // 9 5 
	0.1f,-0.2f,0.0f, // 10 6
	-0.1f,-0.2f,0.0f, // 11 7
	-0.7f,0.4f,0.0f, // 4 0
	0.0f,0.9f,0.0f, // 5 1 
	0.7f,0.4f,0.0f, // 6 2
	-0.1f,0.0f,0.0f, // 12 0
	-0.3f,0.0f,0.0f, // 13 1 
	-0.3f,0.2f,0.0f, // 14 2
	-0.1f,0.2f,0.0f, // 15 3
	0.1f,0.0f,0.0f, // 16 4
	0.3f,0.0f,0.0f, // 13 5
	0.3f,0.2f,0.0f, // 14 6 
	0.1f,0.2f,0.0f, // 15 7
};


GLuint indices_cuadrado[]={
	0,1,
	1,2,
	2,3,
	3,4,
	4,5,
	5,6,
	6,7,
	7,0,
	0,5,
	8,9, // ACA 10->8 11->9 12 -> 10 13 -> 11 14 -> 12 15 -> 13 16 -> 14 17 -> 15
	9,10,
	10,11,
	11,8,
	12,13,
	13,14,
	14,15,
	15,12
};

GLuint indices_triang[]={
	0,1,
	1,2,
	2,0
};

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}

void UpdateMovement_cuadrado(float mov[]){
	for(int i=0;i<16*3;i+=3){
		vector_base_cuadrado[i]+=mov[0];
		vector_base_cuadrado[i+1]+=mov[1];
		vector_base_cuadrado[i+2]+=mov[2];
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBO_house);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vector_base_cuadrado),vector_base_cuadrado);
}

void UpdateMovement_triang(float mov[]){
	for(int i=0;i<9;i+=3){
		vector_base_triang[i]+=mov[0];
		vector_base_triang[i+1]+=mov[1];
		vector_base_triang[i+2]+=mov[2];
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBO_triang);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vector_base_triang),vector_base_triang);
}

void UpdateMovement_points(float mov[]){
	for(int i=0;i<19*3;i+=3){
		vector_base[i]+=mov[0];
		vector_base[i+1]+=mov[1];
		vector_base[i+2]+=mov[2];
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBO_window);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vector_base),vector_base);
}

void key_callback(GLFWwindow* window,int key,int scan,int action,int mods){
	switch(key){
		case GLFW_KEY_ESCAPE:{
			std::cout << "ESC presionado saliendo..." << std::endl;
			glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
		}
		case GLFW_KEY_C:{
			if(mods & GLFW_MOD_CONTROL){
				std::cout << "CTRL+C presionado saliendo..." << std::endl;
				glfwSetWindowShouldClose(window,GLFW_TRUE);
			}
			break;
		}
		case GLFW_KEY_UP:{
			float mov[]={0.0f,0.1f,0.0f};
			UpdateMovement_points(mov);
			UpdateMovement_cuadrado(mov);
			UpdateMovement_triang(mov);
			break; 
		}
		case GLFW_KEY_DOWN:{
			float mov[]={0.0f,-0.1f,0.0f};
			UpdateMovement_points(mov);
			UpdateMovement_cuadrado(mov);
			UpdateMovement_triang(mov);
			break;
		}
		case GLFW_KEY_RIGHT:{
			float mov[]={0.1f,0.0f,0.0f};
			UpdateMovement_points(mov);
			UpdateMovement_cuadrado(mov);
			UpdateMovement_triang(mov);
			break;
		}
		case GLFW_KEY_LEFT:{
			float mov[]={-0.1f,0.0f,0.0f};
			UpdateMovement_points(mov);
			UpdateMovement_cuadrado(mov);
			UpdateMovement_triang(mov);
			break;
		}
		default:{
			break;
		}
	}
}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window=glfwCreateWindow(800,600,"Animación basica",NULL,NULL);
	if(!window){
		std::cout << "Windows didn't charge" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGL(glfwGetProcAddress)){
		std::cout << "GLAD failed :( " << std::endl;
		return -1;
	}
	
	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	
	int sucess;
	char infoLog[512];
	
	
	vertex_shad_id=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shad_id,1,&vertex_shader,NULL);
	glCompileShader(vertex_shad_id);
	
	glGetShaderiv(vertex_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertex_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN VERTEX SHADER ->" << infoLog << std::endl;
	}
	
	fragment_shad_id=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shad_id,1,&fragment_shader_casa,NULL);
	glCompileShader(fragment_shad_id);
	
	glGetShaderiv(fragment_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(fragment_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN FRAGMENT SHADER -> " << infoLog << std::endl;
	}
	
	program_id_cuadrado=glCreateProgram();
	glAttachShader(program_id_cuadrado,vertex_shad_id);
	glAttachShader(program_id_cuadrado,fragment_shad_id);
	glLinkProgram(program_id_cuadrado);
	
	glGetProgramiv(program_id_cuadrado,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(program_id_cuadrado,512,NULL,infoLog);
		std::cout << "ERROR IN LINKING SHADERS -> " << infoLog << std::endl;
	}
	
	glDeleteShader(vertex_shad_id);
	glDeleteShader(fragment_shad_id);
	
	vertex_shad_id=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shad_id,1,&vertex_shader,NULL);
	glCompileShader(vertex_shad_id);
	
	glGetShaderiv(vertex_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertex_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN VERTEX SHADER ->" << infoLog << std::endl;
	}
	
	fragment_shad_id=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shad_id,1,&fragment_shader_techo,NULL);
	glCompileShader(fragment_shad_id);
	
	glGetShaderiv(fragment_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(fragment_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN FRAGMENT SHADER -> " << infoLog << std::endl;
	}
	
	program_id_triang=glCreateProgram();
	glAttachShader(program_id_triang,vertex_shad_id);
	glAttachShader(program_id_triang,fragment_shad_id);
	glLinkProgram(program_id_triang);
	
	glGetProgramiv(program_id_triang,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(program_id_triang,512,NULL,infoLog);
		std::cout << "ERROR IN LINKING SHADERS -> " << infoLog << std::endl;
	}
	
	glDeleteShader(vertex_shad_id);
	glDeleteShader(fragment_shad_id);
	
	vertex_shad_id=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shad_id,1,&vertex_shader,NULL);
	glCompileShader(vertex_shad_id);
	
	glGetShaderiv(vertex_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertex_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN VERTEX SHADER ->" << infoLog << std::endl;
	}
	
	fragment_shad_id=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shad_id,1,&fragment_shader_vert,NULL);
	glCompileShader(fragment_shad_id);
	
	glGetShaderiv(fragment_shad_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(fragment_shad_id,512,NULL,infoLog);
		std::cout << "ERROR IN FRAGMENT SHADER -> " << infoLog << std::endl;
	}
	
	program_id=glCreateProgram();
	glAttachShader(program_id,vertex_shad_id);
	glAttachShader(program_id,fragment_shad_id);
	glLinkProgram(program_id);
	
	glGetProgramiv(program_id,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(program_id,512,NULL,infoLog);
		std::cout << "ERROR IN LINKING SHADERS -> " << infoLog << std::endl;
	}
	
	glDeleteShader(vertex_shad_id);
	glDeleteShader(fragment_shad_id);
	
	
	
	glGenVertexArrays(1,&VAO_triang);
	glGenBuffers(1,&VBO_triang);
	glGenBuffers(1,&EBO_triang);
	
	glBindVertexArray(VAO_triang);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO_triang);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vector_base_triang),vector_base_triang,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_triang);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_triang),indices_triang,GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0); // Para poder configurar otro VAO
	
	glGenVertexArrays(1,&VAO_house);
	glGenBuffers(1,&VBO_house);
	glGenBuffers(1,&EBO_house);
	
	glBindVertexArray(VAO_house);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO_house);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vector_base_cuadrado),vector_base_cuadrado,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_house);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_cuadrado),indices_cuadrado,GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0); // Para poder configurar otro VAO
	
	
	glGenVertexArrays(1,&VAO_window);
	glGenBuffers(1,&VBO_window);
	glGenBuffers(1,&EBO_window);
	
	glBindVertexArray(VAO_window);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO_window);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vector_base),vector_base,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0); // Para poder configurar otro VAO
	
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(program_id_cuadrado);
		glBindVertexArray(VAO_house);
		glDrawElements(GL_LINES,34,GL_UNSIGNED_INT,0);
		
		glUseProgram(program_id);
		glBindVertexArray(VAO_window);
		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, 19);
		
		glUseProgram(program_id_triang);
		glBindVertexArray(VAO_triang);
		glDrawElements(GL_LINES,6,GL_UNSIGNED_INT,0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
}

