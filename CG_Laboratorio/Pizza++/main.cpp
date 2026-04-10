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

#include "Sector.h"

Circle circulo_inicial;
std::unordered_map<std::string,GLuint> mapa;
std::vector<float> vertices;
std::vector<GLuint> indices;
GLuint VAO,VBO,EBO;
const float RADIUS=0.5f;
std::string input="";
unsigned int NUM_REBANADAS=4,SELECT_REBANDA=-1;
bool configurando=false,select_rebanada=false,queueing=false;



const char* vertex_shader_code="#version 330 core\n"
"layout (location=0) in vec2 aVec;\n"
"uniform mat4 model;\n"
"void main(){\n"
"	gl_Position=model*vec4(aVec.xy,0.0f,1.0f);\n"
"}\n";

const char* fragment_shader_border_code="#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor=vec4(217.0f/255.0f,129.0f/255.0f,63.0f/255.0f,1.0f);\n"	
"}\n";

const char* fragment_shader_pizza_code="#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor=vec4(234.0f/255.0f,184.0f/255.0f,133.0f/255.0f,1.0f);\n"	
"}\n";

const char* fragment_shader_pepperoni_code="#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor=vec4(177.0f/255.0f,58.0f/255.0f,18.0f/255.0f,1.0f);\n"
"}\n";

const char* fragment_shader_pina_code="#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor=vec4(242.0f/255.0f,197.0f/255.0f,13.0f/255.0f,1.0f);\n"
"}\n";

const char* fragment_shader_oregano_code="#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor=vec4(135.0f/255.0f,150.0f/255.0f,80.0f/255.0f,1.0f);\n"
"}\n";


void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}



void set_Vs(){
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(),GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),indices.data(),GL_DYNAMIC_DRAW);
	
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
					if(num >= 0 && num < static_cast<int>(circulo_inicial.sectores.size())){
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
			circulo_inicial.Generate_Circle(NUM_REBANADAS,RADIUS);
			circulo_inicial.Generate_Vertices_OpenGL(vertices,indices);
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
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('a',0.0f,0.1f);
			break;
		}
		case GLFW_KEY_DOWN:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('a',0.0f,-0.1f);
			break;
		}
		case GLFW_KEY_RIGHT:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('a',0.1f,0.0f);
			break;
		}
		case GLFW_KEY_LEFT:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('a',-0.1f,0.0f);
			break;
		}
		case GLFW_KEY_D:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('d',90);
			break;
		}
		case GLFW_KEY_F:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('f',90);
			break;
		}
		case GLFW_KEY_G:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('g',1.1,1.1);
			break;
		}
		case GLFW_KEY_H:{
			if(SELECT_REBANDA == -1){
				std::cout << " REBANADA SIN SELECCIONAR " << std::endl;
				break;
			}
			Matrix* mat=circulo_inicial.sectores[SELECT_REBANDA].Get_Matrix();
			mat->UpdateView('g',0.9,0.9);
			break;
		}
		case GLFW_KEY_X:{
			break;
		}
		default:{
			break;
		}
	}
}

void set_Programs(std::string type_name,const char* fragment_shader_code){
	GLuint vertex_shader_id,fragment_shader_id,program_id;
	int sucess;
	char infoLog[512];
	
	
	vertex_shader_id=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id,1,&vertex_shader_code,NULL);
	glCompileShader(vertex_shader_id);
	
	glGetShaderiv(vertex_shader_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertex_shader_id,512,NULL,infoLog);
		std::cout << "Vertex shader failed error -> " << infoLog << std::endl;
	}
	
	fragment_shader_id=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id,1,&fragment_shader_code,NULL);
	glCompileShader(fragment_shader_id);
	
	glGetShaderiv(fragment_shader_id,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(fragment_shader_id,512,NULL,infoLog);
		std::cout << "fragment shader { " << type_name << "} failed error -> " << infoLog << std::endl;
	}
	
	program_id=glCreateProgram();
	glAttachShader(program_id,vertex_shader_id);
	glAttachShader(program_id,fragment_shader_id);
	glLinkProgram(program_id);
	
	glGetProgramiv(program_id,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(program_id,512,NULL,infoLog);
		std::cout << "Linking program failed :( error -> " << infoLog << std::endl;
		glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        glDeleteProgram(program_id);
        return;
	}
	
	mapa[type_name]=program_id;
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	
}




void print_menu() {
    std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Pizza          |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar pizza (default=4)   |" << std::endl;
    std::cout << "|  W. Configurar                  |" << std::endl;
    std::cout << "|  R. Seleccion parte (0-N)       |" << std::endl;
	std::cout << "|  d. Rotar (0.1)                 |" << std::endl;
	std::cout << "|  f. Rotar inverso (0.1)         |" << std::endl;
	std::cout << "|  g. Escalar (1.1)               |" << std::endl;
	std::cout << "|  h. Escalar inverso (0.9)       |" << std::endl;
	std::cout << "|  x. Queue de animacion          |" << std::endl;
    std::cout << "|  4. Mover arriba (Flecha arr)   |" << std::endl;
    std::cout << "|  5. Mover abajo (Flecha abj)    |" << std::endl;
    std::cout << "|  6. Mover derecha (Flecha der)  |" << std::endl;
	std::cout << "|  7. Mover izquierda (Flecha izq)|" << std::endl;
    std::cout << "|  8. Salir (ESC o CTRL+C)        |" << std::endl;
    std::cout << "===================================" << std::endl;
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
	
	set_Programs("Borde",fragment_shader_border_code);
	set_Programs("Pizza",fragment_shader_pizza_code);
	set_Programs("Pina",fragment_shader_pina_code);
	set_Programs("Pepperoni",fragment_shader_pepperoni_code);
	set_Programs("Oregano",fragment_shader_oregano_code);
	
	print_menu();
	
	circulo_inicial.Generate_Circle(NUM_REBANADAS,RADIUS);
	circulo_inicial.Generate_Vertices_OpenGL(vertices,indices);
	
	set_Vs();
	
	GLuint pizzaLoc   = glGetUniformLocation(mapa["Pizza"], "model");
	GLuint bordeLoc   = glGetUniformLocation(mapa["Borde"], "model");
	GLuint pepperLoc  = glGetUniformLocation(mapa["Pepperoni"], "model");
	GLuint oreganoLoc = glGetUniformLocation(mapa["Oregano"], "model");
	GLuint pinaLoc = glGetUniformLocation(mapa["Pina"],"model");
	

	
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		GLuint loc = glGetUniformLocation(mapa["Borde"], "model");
		glBindVertexArray(VAO);
		for(int i=0;i<circulo_inicial.sectores.size();i++){
			Sector& s= circulo_inicial.sectores[i];
			
			glUseProgram(mapa["Pizza"]);
			glUniformMatrix4fv(pizzaLoc, 1, GL_FALSE, s.Get_Matrix()->matrix.data());
			glDrawElements(GL_TRIANGLES,(s.triang_end - s.triang_ini),GL_UNSIGNED_INT,(void*)(s.triang_ini * sizeof(unsigned int)));

			
			glUseProgram(mapa["Borde"]);
			glLineWidth(8.0f);
			glUniformMatrix4fv(bordeLoc, 1, GL_FALSE, s.Get_Matrix()->matrix.data());
			glDrawElements(GL_LINES,(s.border_end - s.border_ini),GL_UNSIGNED_INT,(void*)(s.border_ini * sizeof(unsigned int)));
			glDrawElements(GL_LINES,(s.line_end - s.line_ini),GL_UNSIGNED_INT,(void*)(s.line_ini * sizeof(unsigned int)));
			
			glUseProgram(mapa["Pepperoni"]);
			glUniformMatrix4fv(pepperLoc, 1, GL_FALSE, s.Get_Matrix()->matrix.data());
			glDrawElements(GL_TRIANGLES,(s.pepperoni_end - s.pepperoni_ini),GL_UNSIGNED_INT,(void*)(s.pepperoni_ini * sizeof(unsigned int)));
			
			glUseProgram(mapa["Pina"]);
			glUniformMatrix4fv(pinaLoc, 1, GL_FALSE, s.Get_Matrix()->matrix.data());
			glDrawElements(GL_TRIANGLES,(s.pina_end-s.pina_ini),GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * s.pina_ini));
			
			glUseProgram(mapa["Oregano"]);
			glPointSize(7.0f);
			glUniformMatrix4fv(oreganoLoc, 1, GL_FALSE, s.Get_Matrix()->matrix.data());
			glDrawElements(GL_POINTS,(s.oregano_end - s.oregano_ini),GL_UNSIGNED_INT,(void*)(s.oregano_ini * sizeof(unsigned int)));
			
		}
		glBindVertexArray(0);
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
}
