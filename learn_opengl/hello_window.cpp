#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
// prototype func for framebuffer resizing callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// prototype func for input handling
void processInput(GLFWwindow* window);

int main() {
	// init GLFW
	glfwInit();
	// set compatibility to OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	// use core prfile (removes backward compatible features)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	// terminate if window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// current thread main context = window context - ie. make all future OpenGL functions refer to this window's context (state)
	glfwMakeContextCurrent(window);

	// initialise GLAD before calling any OpenGL function to resolve function name lookup
	// tells GLAD to look to GLFW's address getter to find functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	// set viewport - size and location of our rendering within the window context
	// we have a world context - anything drawn is projected onto the current viewport (ie. different aspect ratio between window and viewport would squish/pull drawn objects)
	glViewport(0, 0, 800, 600); // x = 0, y = 0 (bottom), width = 800, height = 600

	// if we want to resize the window, we should resize the viewport accordingly
	// we have defined a helper function to do this in framebuffer_size_callback() - this performs the resizing of the viewport
	// callback functions should be 'registered' after window creation and before render loop is created
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // call framebuffer_size_callback() whenever framebuffer (dims) of current window is resized

	// render loop
	// each iteration is 1 frame
	while (!glfwWindowShouldClose(window)) {
		
		// check input
		processInput(window); // check if should close on this iteration - should happen first so that input is processed and immediately attempt to render the result onto the screen on each frame

		// perform rendering commands
		// we want to clear the screen on every new frame to allow for new rendering to take place - no point in keeping the previous frame's drawing
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // defines the clear color
		glClear(GL_COLOR_BUFFER_BIT); // set color buffer bit to clear color

		// swap buffers, check and call events
		glfwSwapBuffers(window); // this is buffering on each iteration of the while loop with 2 buffers swapped between (double buffering)
		glfwPollEvents(); // checks if any events are triggered, update window state, call any callbacks etc.
	}

	// terminate GLFW, dealloc resources in mem
	glfwTerminate();
	return 0;




}

// function for callback function on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); // tell program that viewport is to be resized to new width/height
}

// function to process input to window
void processInput(GLFWwindow* window) {
	// if escape pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); // close window signal
	}
}