
#include <glad/glad.h> // Be sure to include GLAD before GLFW
#include <GLFW/glfw3.h>

// this function is registered to GLFW and will be called automajically whenever the window is resized.
// GLFW knows to provide the appropriate width and height matching the new size of the window when she calls this function.
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// We also want to have some form of input control in GLFW and we can achieve this with several of GLFW's input functions.
// We'll be using GLFW's glfwGetKey function that takes the window as input together with a key.
// The function returns whether this key is currently being pressed.
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// Initialize GLFW.
	// Specify our OpenGL version as 3.3.
	// We also tell GLFW we want to explicitly use the core-profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// This window object holds all the windowing data and is required by most of GLFW's other functions.
	// We tell GLFW to make the context of our window the main context on the current thread.
	int initWidth = 1920;
	int initHeight = 1080;
	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "We love OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// We want to initialize GLAD before we call any OpenGL function.
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// We have to tell OpenGL the size of the rendering window,
	// so OpenGL knows how we want to display the data and coordinates with respect to the window.
	glViewport(0, 0, initWidth, initHeight);

	// We have to tell GLFW we want to call this function on every window resize by registering it.
	// We register callback functions after we've created the window and before the render loop is initiated.
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW has been instructed to close.
	// The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events),
	// updates the window state, and calls the corresponding functions (which we can register via callback methods).
	// The glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window),
	// that is used to render to during this render iteration and show it as output to the screen.
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// As soon as we exit the render loop we would like to properly clean/delete all of GLFW's resources that were allocated.
	glfwTerminate();
	return 0;
}

