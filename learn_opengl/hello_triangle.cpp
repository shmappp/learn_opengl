#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shaders

// vertex shader
// this shader code simply takes in 3d coords and spits them out as a vec4 unmapped
const char* vertexShaderSource = "#version 330 core\n" // version and profile type
"layout(location = 0) in vec3 aPos;\n" // location of input variable and input variable type
"void main() {\n" // shader func
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // just map across input values
"}\0";

// fragment shader
// outputs rgba info for vertices
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // output frag color
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main() {

	// init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create glfw window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hello triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// link glad function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	// make shaders
	// vertex shader code
	// create opengl shader object - requires id as all opengl objects
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// bind shader source code to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile vertex shader
	glCompileShader(vertexShader);
	// check compilation success
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// fragment shader code
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	
	// create shader program - linked ver of all shaders combined
	// will require shaders to be linked across
	unsigned int shaderProgram; // id store
	shaderProgram = glCreateProgram(); // gen id and program
	// link
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check linking success
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
	}
	// once linked, can delete individual shader objects from memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	
	// create our vertices
	// opengl only renders normalised 3d coords b/w -1 and 1
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	
	// set up VBO, VAO, EBO
	
	// opengl vertex array object
	// used to simplify the drawing of vertices informed by data held in VBOs by storing pointer and state data for our vertex attributes
	// allows drawing by simply binding VAO rather than having to continually respecify how to access VBO attribute data
	unsigned int VAO;

	// opengl vertex buffer object - stores vertices in gpu memory
	// must generate with an id
	unsigned int VBO;

	// gen arrays and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // this is a generic buffer object, will assign the corresponding buffer id to VBO


	// bind buffers and store data
	// bind the VAO first, so that we can configure vertex attributes for the following bound VBOs
	glBindVertexArray(VAO); // !!!IMPORTANT!!! the VAO, once bound, is like a WATCHER - it will intercept all future bind calls alongside their corresponding vertex attribute pointers

	// we have to bind our new VBO to GL_ARRAY_BUFFER
	// binding our VBO to the GL_ARRAY_BUFFER means that each vertex attribute will take their data from this VBO as it is linked to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// store vertices in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // last argument refers to how the data will be stored in the buffer based on usage - static draw for data that won't change very much but will be used a lot

	// all of the following data is stored in the VAO as the preferred settings for opengl to interpret the vertex attribute data alongside the data in the GL_ARRAY_BUFFER
	// inform opengl on how to interpret vertex data based on vertex buffer data format
	// our vertex buffer is tightly packed (no space between each instance of the pointer)
	glVertexAttribPointer(0, // informs opengl about the 0 location vertex attribute
		3, // it has 3 values as it's vec3
		GL_FLOAT, // comprised of floats 
		GL_FALSE, // no normalisation needed
		3 * sizeof(float), // stride = 3 floats space
		(void*)0 // no (ie. void) buffer offset
	);
	// enable the vertex attribute for location 0
	glEnableVertexAttribArray(0);

	// since we have bound the VBO data, the description of data from glVertexAttribPointer and the enable call to the VAO, we can unbind the VBO from the GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// after performing all our binds for all our VBOs, we may unbind the VAO, stopping it from collecting VBO bind calls and attrib pointers
	// this prevents accidental modification of this VAO, but is not necessary since modifying another VAO would require another call to glBindVertexArray
	glBindVertexArray(0);

	// render loop
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our triangle
		// activate program
		glUseProgram(shaderProgram); // any subsequent shader and rendering call will use shaderProgram
		glBindVertexArray(VAO); // bind our VAO with the VBO data and pointer info
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // optional unbind

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// dealloc all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
