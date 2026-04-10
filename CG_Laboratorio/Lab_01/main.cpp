#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource_1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
    "}\n\0";

//Lineas
const char *fragmentShaderSource_2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";

//Vertices
const char *fragmentShaderSource_3 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	
	
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource_1, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	
	
    // link shaders
    unsigned int shaderProgram_1 = glCreateProgram();
    glAttachShader(shaderProgram_1, vertexShader);
    glAttachShader(shaderProgram_1, fragmentShader);
    glLinkProgram(shaderProgram_1);
    // check for linking errors
    glGetProgramiv(shaderProgram_1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	
	// fragment shader
    unsigned int fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_2, 1, &fragmentShaderSource_2, NULL);
    glCompileShader(fragmentShader_2);
    // check for shader compile errors
    glGetShaderiv(fragmentShader_2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	
	unsigned int fragmentShader_3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_3, 1, &fragmentShaderSource_3, NULL);
    glCompileShader(fragmentShader_3);
    // check for shader compile errors
    glGetShaderiv(fragmentShader_3, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_3, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


	// link shaders
    unsigned int shaderProgram_2 = glCreateProgram();
    glAttachShader(shaderProgram_2, vertexShader);
    glAttachShader(shaderProgram_2, fragmentShader_2);
    glLinkProgram(shaderProgram_2);
    // check for linking errors
    glGetProgramiv(shaderProgram_2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
	
	// link shaders
    unsigned int shaderProgram_3 = glCreateProgram();
    glAttachShader(shaderProgram_3, vertexShader);
    glAttachShader(shaderProgram_3, fragmentShader_3);
    glLinkProgram(shaderProgram_3);
    // check for linking errors
    glGetProgramiv(shaderProgram_3, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_3, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
	
	
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader_2);
	glDeleteShader(fragmentShader_3);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         -0.8f,  0.0f, 0.0f,  // izquierda
         0.0f,  0.0f, 0.0f,  // derecha
         -0.4f, 0.8f, 0.0f  // centro
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
    };
	
	float vertices_2[] = {
         0.2f,  0.0f, 0.0f,  // esquina izquieda
         0.2f,  0.6f, 0.0f,  // izquierda arriba
		 0.9f, 0.6f, 0.0f, // arriba derecha
         0.9f, 0.0f, 0.0f  // esquina derecha
    };
    unsigned int indices_2_triangulo[] = {  // note that we start from 0!
        0, 1, 2,
		0, 2, 3
    };
	
	
    unsigned int VBO, VAO, EBO;
	unsigned int VBO_2,VAO_2,EBO_2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

	glGenVertexArrays(1,&VAO_2);
	glGenBuffers(1,&VBO_2);
	glGenBuffers(1,&EBO_2);
	
	glBindVertexArray(VAO_2);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO_2);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_2),vertices_2,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_2_triangulo),indices_2_triangulo,GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram_1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderProgram_2);
		glBindVertexArray(VAO);
		glLineWidth(2.0f);
		
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderProgram_3);
		glBindVertexArray(VAO);
		glPointSize(10.0f);
		glDrawElements(GL_POINTS,3,GL_UNSIGNED_INT,0);
		
		
		// Rectangulo
		glUseProgram(shaderProgram_1);
        glBindVertexArray(VAO_2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glUseProgram(shaderProgram_2);
		glBindVertexArray(VAO_2);
		glLineWidth(2.0f);
		
        glDrawArrays(GL_LINE_LOOP, 0, 4);
		
		glUseProgram(shaderProgram_3);
		glBindVertexArray(VAO_2);
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS,0,4);
		
		
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram_1);
	glDeleteProgram(shaderProgram_2);
	glDeleteProgram(shaderProgram_3);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}