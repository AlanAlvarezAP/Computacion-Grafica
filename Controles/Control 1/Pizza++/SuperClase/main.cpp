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


GLuint VAO,VBO,EBO;



float triangle_vec[]={
	-0.3f,-0.2f,0.0f,1.0f,0.0f,0.0f,
	0.3f,-0.2f,0.0f,0.0f,1.0f,0.0f,
	0.0f,0.4f,0.0f,0.0f,0.0f,1.0f
};

GLuint indices[]={
	0,1,2
};


void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
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
	
	
	
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle_vec),triangle_vec,GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0); // Para poder configurar otro VAO
	
	
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//glUseProgram(program_id);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
}
