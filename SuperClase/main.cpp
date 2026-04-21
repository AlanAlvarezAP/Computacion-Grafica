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

#include "Shape.h"
#include "Builder.h"

World* mundito;
GLuint VAO,VBO,EBO;
unsigned int NUM_REBANADAS=4,SELECT_REBANDA=-1;
std::string input="";
bool configurando=false,select_rebanada=false,queueing=false,everyone_scale=false;
char CURRENT_AXIS = 'z';


void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}
void set_Vs(){
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,mundito->all_vertices.size()*sizeof(float),mundito->all_vertices.data(),GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,mundito->all_EBOs.size()*sizeof(unsigned int),mundito->all_EBOs.data(),GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
}


void key_callback(GLFWwindow* window,int key,int scan,int action,int mods){
	if(action != GLFW_PRESS){
		return;
	}
	switch(key){
		case GLFW_KEY_ESCAPE:{
			if(!input.empty()){
				input = "";
				configurando = false;
				std::cout << "Cancelado." << std::endl;
			} else {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			break;
		}
		case GLFW_KEY_C:{
			if(!input.empty()){
				input = "";
				configurando = false;
				std::cout << "Cancelado." << std::endl;
			}else{
				if(mods & GLFW_MOD_CONTROL){
					std::cout << "CTRL+C presionado saliendo..." << std::endl;
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				}
			}
			
			break;
		}
		case GLFW_KEY_0:
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:
		case GLFW_KEY_7:
		case GLFW_KEY_8:
		case GLFW_KEY_9:{
			input+=(char)('0' + (key - GLFW_KEY_0));
			std::cout << " Se leyo " << input << std::endl;
			break;
		}
		case GLFW_KEY_BACKSPACE:{
			if(!input.empty()){
				input.pop_back();
			}
			break;
		}
		case GLFW_KEY_ENTER:{
			if(!input.empty()){
				int num=std::stoi(input);
				if(configurando){
					if(num>0){
						NUM_REBANADAS=num;
						std::cout << "Rebanadas configuradas: " << num << std::endl;
					}else{
						std::cout << " Numero invalido < 0 " << std::endl;
					}
					configurando=false;
				}else if(select_rebanada){
					if(num >= 0 && num < static_cast<int>(mundito->root->children.size())){
						std::cout << "Confirmando rebanada... " << input << std::endl;
						SELECT_REBANDA=num;
					}
					else{
						std::cout << "Numero invalido o rebanada no existente :( " << std::endl;
					}
					select_rebanada=false;
				}
			}
			input="";
			break;
		}
		case GLFW_KEY_Q:{
			//Builder::BuildPizzaScene(mundito,NUM_REBANADAS);
			Builder::BuildPyramidScene(mundito,1.0f);
			set_Vs();
			break;
		}
		case GLFW_KEY_W:{
			input = "";
			std::cout << "Dame la cantidad de rebanadas: " << std::endl;
			configurando=true;
			break;
		}
		case GLFW_KEY_R:{
			SELECT_REBANDA=-1;
			input = "";
			std::cout << "Dame la pizza a mover ... " << std::endl;
			select_rebanada=true;
			break;
		}
		case GLFW_KEY_UP:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('a',0.0f,0.1f);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('a',0.0f,0.1f);
			break;
		}
		case GLFW_KEY_DOWN:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('a',0.0f,-0.1f);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('a',0.0f,-0.1f);
			break;
		}
		case GLFW_KEY_RIGHT:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('a',0.1f,0.0f);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('a',0.1f,0.0f);
			break;
		}
		case GLFW_KEY_LEFT:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('a',-0.1f,0.0f);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('a',-0.1f,0.0f);
			break;
		}
		case GLFW_KEY_D:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('d',10.0f,0.0f,0.0f,CURRENT_AXIS);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('d',10.0f,0.0f,0.0f,CURRENT_AXIS);
			break;
		}
		case GLFW_KEY_F:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('f',10.0f,0.0f,CURRENT_AXIS);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('f',10.0f,0.0f,0.0f,CURRENT_AXIS);
			break;
		}
		case GLFW_KEY_G:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('g',1.1,1.1);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('g',1.1,1.1);
			break;
		}
		case GLFW_KEY_H:{
			if(SELECT_REBANDA == -1){
				Matrix* mat=&(mundito->root->Mat);
				mat->UpdateView('g',0.9,0.9);
				break;
			}
			Matrix* mat=&(mundito->root->children[SELECT_REBANDA]->Mat);
			mat->UpdateView('g',0.9,0.9);
			break;
		}
		case GLFW_KEY_X:{
			CURRENT_AXIS='x';
			std::cout << "Eje actual:X" << std::endl;
			break;
		}
		case GLFW_KEY_Y:{
			CURRENT_AXIS='y';
			std::cout << "Eje actual:Y" << std::endl;
			break;
		}
		case GLFW_KEY_Z:{
			CURRENT_AXIS='z';
			std::cout << "Eje actual: Z" << std::endl;
			break;
		}
		default:{
			break;
		}
	}
}
void print_menu() {
    std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Pizza          |" << std::endl;
	std::cout << "|     Control 1                   |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar pizza (default=4)   |" << std::endl;
    std::cout << "|  W. Configurar                  |" << std::endl;
    std::cout << "|  R. Seleccion parte (0-N)       |" << std::endl;
	std::cout << "|  d. Rotar (0.1)                 |" << std::endl;
	std::cout << "|  f. Rotar inverso (0.1)         |" << std::endl;
	std::cout << "|  g. Escalar (1.1)               |" << std::endl;
	std::cout << "|  h. Escalar inverso (0.9)       |" << std::endl;
	std::cout << "|  x. Usar eje x                  |" << std::endl;
	std::cout << "|  y. Usar eje y                  |" << std::endl;
	std::cout << "|  z. Usar eje z                  |" << std::endl;
    std::cout << "|  4. Mover arriba (Flecha arr)   |" << std::endl;
    std::cout << "|  5. Mover abajo (Flecha abj)    |" << std::endl;
    std::cout << "|  6. Mover derecha (Flecha der)  |" << std::endl;
	std::cout << "|  7. Mover izquierda (Flecha izq)|" << std::endl;
    std::cout << "|  8. Salir (ESC o CTRL+C)        |" << std::endl;
    std::cout << "===================================" << std::endl;
	std::cout << " AL TERMINAR DE ESCRIBIR LA PARTE O CONFIGURACIÓN DE REBANDAS CONFIRMAR CON ENTER " << std::endl;
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
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	
	print_menu();
	//Builder::BuildPizzaScene(mundito,NUM_REBANADAS);
	Builder::BuildPyramidScene(mundito,1.0f);

	set_Vs();
	//mundito->print(mundito->root);
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(VAO);
		glPointSize(8.0f);
		glLineWidth(8.0f);
        mundito->DrawShape();
		
		glBindVertexArray(0);
		
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	delete mundito;
	return 0;
}
