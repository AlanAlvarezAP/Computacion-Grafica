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

World* mundito=nullptr;
GLuint VAO,VBO,EBO;
unsigned int NUM_REBANADAS=4,SELECT_REBANDA=-1;
std::string input="";
char CURRENT_AXIS = 'z';
Pizza* pizza=nullptr;
Piramid* piramid=nullptr;
Cube* cube=nullptr;
Sphere* sphere = nullptr;
Tower* tower=nullptr;
enum Configuration_type{
	NONE,
	SELECT_REBANADA,
	SET_REBANADA
};

enum Input_status{
	NORMAL,
	CONFIGURANDO
};

enum Scene_Shapes{
	PIZZA,
	PYRAMID,
	CUBE,
	SPHERE,
	TOWER
};

Input_status inputMode=NORMAL;
Scene_Shapes sceneMode=PIZZA;
Configuration_type inputContext=NONE;

// a -> traslacion normal
// s -> traslacion inv
// d -> rotacion normal
// f -> rotacion inv
// g -> escala normal
// h -> escala inv

void alinear(){
	if(pizza){
		mundito->Add_animation(new Animation_Step(piramid, 1.0f, 'd', 90.0f, 'x'));
		while(!mundito->pedidos_norm.empty() || !mundito->pedidos_inv.empty()){
			mundito->Execute_animations(1.0f,'N');
		}
		
	}
	if(piramid){
		mundito->Add_animation(new Animation_Step(piramid, 1.0f, 'd', 90.0f, 'x'));
		while(!mundito->pedidos_norm.empty() || !mundito->pedidos_inv.empty()){
			mundito->Execute_animations(1.0f,'N');
		}
	}
	
}

void pedidos_Sphere(Sphere* sphere){
	
	for(int i=0;i<5;i++){
		mundito->Add_animation(new Animation_Step(piramid, 1.0f, 'g', 1.1f, 'x'));
	}
	
	
	/*mundito->Add_animation(new Animation_Step(piramid, 2.0f, 'f', 180.0f, 'x'));
	for(int i=0;i<10;i++){
		mundito->Add_animation(new Animation_Step(piramid, 1.0f, 'd', 360.0f, 'y'));
	}
	mundito->Add_animation(new Animation_Step(piramid, 2.0f, 'd', 45.0f, 'z'));*/
}

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
	
	if(inputMode == CONFIGURANDO){
		if(key >= GLFW_KEY_0 && key <= GLFW_KEY_9){
			input+=(char)('0' + (key - GLFW_KEY_0));
			std::cout << " Se leyo " << input << std::endl;
			return;
		}
		else if (key == GLFW_KEY_BACKSPACE && !input.empty()){
			input.pop_back();
			return;
		}
		else if(key == GLFW_KEY_ENTER && !input.empty()){
			int num=std::stoi(input);
			if(inputContext == SELECT_REBANADA){
				auto node = mundito->activeSceneNode;

				if(num >= 0 && num < (int)node->children.size()){
					std::cout << "Confirmando seleccion... " << input << std::endl;
					node->SelectPart(num);
				}
				else{
					std::cout << "Numero invalido :( " << std::endl;
					node->SelectPart(-1);
				}
				inputContext=NONE;
			}
			else if(inputContext == SET_REBANADA){
				if(num>0){
					NUM_REBANADAS=num;
					std::cout << "Rebanadas configuradas: " << num << std::endl;
				}else{
					std::cout << " Numero invalido < 0 " << std::endl;
				}
				inputContext=NONE;
			}
			input.clear();
			inputMode=NORMAL;
			return;
		}
	}
	
	switch(key){
		case GLFW_KEY_ESCAPE:{
			if(!input.empty()){
				input.clear();
				inputContext=NONE;
				inputMode=NORMAL;
				std::cout << "Cancelado." << std::endl;
			} else {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			break;
		}
		case GLFW_KEY_C:{
			if(!input.empty()){
				input.clear();
				inputContext=NONE;
				inputMode=NORMAL;
				std::cout << "Cancelado." << std::endl;
			}else{
				if(mods & GLFW_MOD_CONTROL){
					std::cout << "CTRL+C presionado saliendo..." << std::endl;
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				}
			}
			break;
		}
		case GLFW_KEY_1:{
			// Esfera
			Sphere* esfera=dynamic_cast<Sphere*>(tower->children[2]);
			esfera->Mat.UpdateView('g',1.1f,1.1f,1.1f,'x');
			break;
		}
		case GLFW_KEY_2:{
			// Traslacion cubos
			Cube* cubo1=dynamic_cast<Cube*>(tower->children[0]);
			Cube* cubo2=dynamic_cast<Cube*>(tower->children[1]);
			cubo1->Mat.UpdateView('a',0.0f,-0.1f,0.0f,'y');
			cubo2->Mat.UpdateView('a',0.0f,0.1f,0.0f,'y');
			break;
		}
		case GLFW_KEY_3:{
			// Rotacion piramides
			Piramid* pyramid1 = dynamic_cast<Piramid*>(tower->children[3]);
			Piramid* pyramid2 = dynamic_cast<Piramid*>(tower->children[4]);
			
			pyramid1->Mat.UpdateView('d',90.0f,0.0f,0.0f,'x');
			pyramid2->Mat.UpdateView('d',90.0f,0.0f,0.0f,'x');
			break;
		}
		case GLFW_KEY_4:{
			Piramid* pyramid1 = dynamic_cast<Piramid*>(tower->children[3]);
			Piramid* pyramid2 = dynamic_cast<Piramid*>(tower->children[4]);
			
			pyramid1->Mat.UpdateView('d',90.0f,0.0f,0.0f,'y');
			pyramid2->Mat.UpdateView('d',90.0f,0.0f,0.0f,'y');
            break;
		}
		case GLFW_KEY_5:{
			Cube* cubo1=dynamic_cast<Cube*>(tower->children[0]);
			Cube* cubo2=dynamic_cast<Cube*>(tower->children[1]);
			cubo1->Mat.UpdateView('d',10.0f,0.0f,0.0f,'x');
			cubo2->Mat.UpdateView('d',10.0f,0.0f,0.0f,'x');
            break;
		}
		case GLFW_KEY_Q:{
            for (auto c : mundito->root->children){
				delete c;
			}

			mundito->root->children.clear();

            if (sceneMode == PIZZA && pizza){
                pizza=Builder::BuildPizzaScene(mundito, NUM_REBANADAS);
				mundito->activeSceneNode=pizza;
				set_Vs();
			}
            else if (sceneMode == PYRAMID && piramid){
                piramid=Builder::BuildPyramidScene(mundito, 1.0f);
				mundito->activeSceneNode=piramid;
				set_Vs();
			}else if (sceneMode == CUBE && cube) {
				cube = Builder::BuildCubeScene(mundito, {0.0f,0.0f,0.0f});
				mundito->activeSceneNode = cube;
				set_Vs();
			}else if(sceneMode == TOWER && tower){
				tower = Builder::BuildTowerScene(mundito);
				mundito->activeSceneNode = tower;
				set_Vs();
			}
			else{
				std::cout << " NO EXISTE NADA " << std::endl;
			}
            
			break;
		}
		case GLFW_KEY_W:{
			input.clear();
			std::cout << "Dame la cantidad de rebanadas: " << std::endl;
			inputMode=CONFIGURANDO;
			inputContext=SET_REBANADA;
			break;
		}
		case GLFW_KEY_R:{
			SELECT_REBANDA=-1;
			input.clear();
			std::cout << "Dame la pizza a mover ... " << std::endl;
			inputMode=CONFIGURANDO;
			inputContext=SELECT_REBANADA;
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
		case GLFW_KEY_T:{
			if(mundito && mundito->activeSceneNode){
				mundito->activeSceneNode->EditMode();
			}
			break;
		}
		default:{
			break;
		}
	}
	if(inputMode == NORMAL){
		if(mundito && mundito->activeSceneNode){
			mundito->activeSceneNode->handleKey(key,mods,CURRENT_AXIS);
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
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	
	mundito = new World();
	
	/*std::cout << "CONSTRUYENDO PIZZA " << std::endl;
	pizza = Builder::BuildPizzaScene(mundito,NUM_REBANADAS);*/

	//piramid = Builder::BuildPyramidScene(mundito,1.0f);
	//cube = Builder::BuildCubeScene(mundito,{0.0f,0.0f,0.0f});
	//sphere=Builder::BuildSphereScene(mundito,0.5f);
	tower = Builder::BuildTowerScene(mundito);
	mundito->activeSceneNode= tower;

	mundito->activeSceneNode->printMenu();

	set_Vs();
	//mundito->print(mundito->root);
	glEnable(GL_DEPTH_TEST);
	
	alinear();

	float lastTime=glfwGetTime();

	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float NowTime=glfwGetTime();
		float dt=NowTime-lastTime;
		lastTime=NowTime;
		glBindVertexArray(VAO);
		glPointSize(8.0f);
		glLineWidth(8.0f);
		
		mundito->Execute_animations(dt,'S');
		
        mundito->DrawShape();
		
		glBindVertexArray(0);
		
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	delete mundito;
	return 0;
}
