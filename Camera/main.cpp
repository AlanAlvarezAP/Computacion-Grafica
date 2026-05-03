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
unsigned int NUM_REBANADAS=4,SELECT_REBANDA=0;
std::string input="";
char CURRENT_AXIS = 'z';
Pizza* pizza=nullptr;
Piramid* piramid=nullptr;
Cube* cube=nullptr;
Sphere* sphere = nullptr;
Tower* tower=nullptr;
Robot* robot=nullptr;
Camera* cam=nullptr;
bool First_cam=true;
float dt=0.0f,lastX=0.0f,lastY=0.0f;
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
    TOWER,
    ROBOT
};

Input_status inputMode=NORMAL;
Configuration_type inputContext=NONE;
int currentSceneIndex = 5;
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (First_cam){
        lastX = xpos;
        lastY = ypos;
        First_cam = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cam->ProcessMouse(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam->ProcessScroll((float)yoffset);
}
void key_callback(GLFWwindow* window,int key,int scan,int action,int mods){
	if(action != GLFW_PRESS){
		return;
	}
	
	if(key == GLFW_KEY_ESCAPE){
		input.clear();
		inputContext=NONE;
		inputMode=NORMAL;
		std::cout << "Cancelado." << std::endl;
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
			std::cout << input << std::endl;
			int num=std::stoi(input);
			if(inputContext == SELECT_REBANADA){
				auto node = mundito->activeSceneNode;

				if(num >= 0 && num < (int)node->children.size()){
					std::cout << "Confirmando seleccion... " << input << std::endl;
					node->SelectPart(num);
					mundito->activeSceneNode->editWhole=false;
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
		return;
	}
	
	switch(key){
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
			if(currentSceneIndex == static_cast<Scene_Shapes>(Scene_Shapes::ROBOT)){
				robot->Walk();
			}else{
				std::cout << "No es robot no se puede caminar" << std::endl;
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
			std::cout << "Dame la parte del grafo a mover ... " << std::endl;
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
		case GLFW_KEY_P:{
			currentSceneIndex =(currentSceneIndex+1)%mundito->root->children.size();
			mundito->activeSceneNode=mundito->root->children[currentSceneIndex];
			std::cout << "Escena actual: "<< currentSceneIndex << std::endl;
			break;
		}
		case GLFW_KEY_G:{
			cam->ProcessKeyboard(LEFT,dt);
			break;
		}
		case GLFW_KEY_H:{
			cam->ProcessKeyboard(FORWARD,dt);
			break;
		}
		case GLFW_KEY_J:{
			cam->ProcessKeyboard(BACKWARD,dt);
			break;
		}
		case GLFW_KEY_K:{
			cam->ProcessKeyboard(RIGHT,dt);
			break;
		}
		default:{
			break;
		}
	}
	if(inputMode == NORMAL){
		if(mundito && mundito->activeSceneNode){
			if(mundito->activeSceneNode->editWhole){
				mundito->root->handleKey(key,mods,CURRENT_AXIS);
			}else{
				mundito->activeSceneNode->handleKey(key,mods,CURRENT_AXIS);
			}
			
		}
	}
	
}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window=glfwCreateWindow(800,800,"Animación robot",NULL,NULL);
	if(!window){
		std::cout << "Windows didn't charge" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	if(!gladLoadGL(glfwGetProcAddress)){
		std::cout << "GLAD failed :( " << std::endl;
		return -1;
	}
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	mundito = new World();
	cam = Builder::BuildCamera();
	
	/*std::cout << "CONSTRUYENDO PIZZA " << std::endl;
	pizza = Builder::BuildPizzaScene(mundito,NUM_REBANADAS);*/

	//piramid = Builder::BuildPyramidScene(mundito,1.0f);
	//cube = Builder::BuildCubeScene(mundito,{0.0f,0.0f,0.0f});
	//sphere=Builder::BuildSphereScene(mundito,0.5f);
	//tower = Builder::BuildTowerScene(mundito);
	robot = Builder::BuildRobotScene(mundito);
	mundito->activeSceneNode= robot;

	mundito->activeSceneNode->printMenu();
	mundito->print(mundito->root);

	set_Vs();
	//mundito->print(mundito->root);
	glEnable(GL_DEPTH_TEST);
	

	float lastTime=glfwGetTime();
	double fpsTime = 0.0;
	int fpsFrames = 0;
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float NowTime=glfwGetTime();
		dt=NowTime-lastTime;
		lastTime=NowTime;
		
		fpsTime += dt;
		fpsFrames++;

		if (fpsTime >= 1.0) {
			double fps = fpsFrames / fpsTime;

			std::string title = "Robot OpenGL - FPS: " + std::to_string((int)fps);
			glfwSetWindowTitle(window, title.c_str());

			fpsFrames = 0;
			fpsTime = 0.0;
		}
		
		glBindVertexArray(VAO);
		glPointSize(4.0f);
		glLineWidth(4.0f);
		
		
		glfwPollEvents();
		
		GLuint viewLoc = glGetUniformLocation(mundito->Shader_global.GetProgram(), "view");
		Matrix view=cam->GetLookAt();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.matrix.data());
		
		mundito->Execute_animations(dt,'S');
		
        mundito->DrawShape();
		
		glBindVertexArray(0);
		
        glfwSwapBuffers(window);
        
    }
	delete mundito;
	return 0;
}
