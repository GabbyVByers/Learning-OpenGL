
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

struct Vertex
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int initWidth = 1920;
	int initHeight = 1080;
	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "We love OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, initWidth, initHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Here we create an array of vertices defining the shape we want to draw
	// We'll want to copy this data into a Vertex Buffer Object on the GPU
	Vertex vertices[] =
	{
		{ -0.7f,  0.7f,  0.0f},
		{  0.7f,  0.0f,  0.0f},
		{ -0.3f, -0.7f,  0.0f}
	};

	// We create a Vertex Buffer Object
	// This is a block of memory on the GPU that will store an array of vertices
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO); // This function is essentially a default constructor for the VBO

	// We also need to make a Vertex Array Object
	// The Vertex Array Object will remember all the changes we make to the state of the Vertex Buffer Object
	// We use the VAO when we actually want to draw something.  It required with OpenGL core, and is self contained right here.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// OpenGL allows us to bind to several buffers at once as long as they have a different buffer type.
	// From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer (our VBO).
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// This function copies the previously defined vertex data into the buffer's memory.
	// The argument GL_STATIC_DRAW means the data will be sent only once and used many times (i.e. every frame).
	// The data will find it's way into our VBO because that is the currently bounded GL_ARRAY_BUFFER object.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// We must tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer.
	// location = 0,
	// this vertex atribute is composed of 3 values, each of type GL_FLOAT,
	// GL_FALSE because we do not want the three values normalized, 
	// The STRIDE, the byte offset between consecutive vertices in the buffer.
	// the OFFSET of where the position data begins in the buffer. Position data is at the start of the array so is just 0. Must cast to (void*).
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Enable the attribute now that it is specified.

	



	// As of now we stored the vertex data within memory on the graphics card as managed by a vertex buffer object named VBO.
	// Next we want to create a vertex and fragment shader that actually processes this data, so let's start building those.

	// We use a helper function to load in source code for each shader into std::strings.
	// We need to convert the std::strings into const char pointers.
	std::string vert = loadSourceFile("shader.vert");
	std::string frag = loadSourceFile("shader.frag");
	const char* vertexShaderSource   = vert.c_str();
	const char* fragmentShaderSource = frag.c_str();

	// First we create a blank shader object.
	// assigning the shader id to glCreateShader(GL_VERTEX_SHADER) marks it as a vertex shader.
	// glShaderSource attaches the raw source code to the vertex shader.
	// glCompileShader(vertexShader) compiles the raw source code so that it can later be invoked by the GPU.
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// The entire process is done a second time to get the fragment shader.
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// A shader program object is the final linked version of multiple shaders combined.
	// To use the recently compiled shaders we have to link them to a shader program object, 
	// and then activate this shader program when rendering objects.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Now we need to attach the previously compiled shaders to the program object and then link them with glLinkProgram.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// The result is a program object that we can activate by calling glUseProgram with the newly created program object as its argument
	glUseProgram(shaderProgram);

	// Shader objects are only needed as intermediates before linking, after that the shaderProgram contains everything.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

