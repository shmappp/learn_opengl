#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	// this shader code simply takes in 3d coords and spits them out as a vec4 unmapped
	const char *vertexShaderSource = "#version 330 core\n" // version and profile type
		"layout(location = 0) in vec3 aPos;\n" // location of input variable and input variable type
		"void main() {\n" // shader func
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // just map across input values
		"}\0";
	// create opengl shader object - requires id as all opengl objects
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// bind shader source code to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile vertex shader
	glCompileShader(vertexShader);


	// opengl only renders normalised 3d coords b/w -1 and 1
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// opengl vertex buffer object - stores vertices in gpu memory
	// must generate with an id
	unsigned int VBO; // buffer id
	glGenBuffers(1, &VBO); // this is a generic buffer object, will assign the corresponding buffer id to VBO
	// we have to bind our new buffer to GL_ARRAY_BUFFER so that it corresponds to a vertex buffer - buffer specified by id
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// store vertices in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // last argument refers to how the data will be stored in the buffer based on usage - static draw for data that won't change very much but will be used a lot

	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}