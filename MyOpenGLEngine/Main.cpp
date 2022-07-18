#include<iostream>
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() \n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1, 0, 0, 1);\n"
"}\0";


int main() 
{
	//Initialize
	glfwInit();

	//triangle creation
	GLfloat vertices[] =
	{
		-.5f,-.5f, 0,
		.5f, -0, 0,
		0, .5f, 0
	};

	//Create Window of specified size, title
	auto window = glfwCreateWindow(1024, 1024, "Title", NULL, NULL);

	//uh oh
	if(window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	//i guess focus the window??
	glfwMakeContextCurrent(window);

	//glad functions assemble!
	gladLoadGL();

	

	//this defines the coordinate system for the viewport, in this case, -1 will be 0 and 1 will be 1024 when we draw stuff
	glViewport(0, 0, 1024, 1024);

	// we create the vertex shader object from the source and compile it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//same but with the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	

	//we create a shader program and attach the copiled shaders to out program.
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//we delete the shaders as they're already in the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vertexArrayObject, vertexBuffer;

	//we generate VAO and VBO
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);


	//binding objects so we can configure them and OpenGL knows how to read the VBO
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	//stream -> modify vertices once, use many times
	//static -> modify vertices once, use many many times
	//dynamic -> modify vertices many times, use many many times

	//draw -> vertices will be used to draw an image on the screen
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//we use this to communicate with the vertex shader from the outside
	glVertexAttribPointer(0, 3 /*number of coords per vertex*/, GL_FLOAT /*type of data*/, GL_FALSE, 3 * sizeof(float) /*size of each vertex*/, nullptr /*space between vertices in memory*/);
	glEnableVertexAttribArray(0);

	//ordering is important as well
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	////set the color we want to clear the buffer with and clear it
	//glClearColor(0, 0, 1, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	////swap buffers so the blue buffer is now in front
	//glfwSwapBuffers(window);

	//process window events
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);

		////experimentation -> change uniform variables after glUseProgram
		//
		//GLint timeLocation = glGetUniformLocation(shaderProgram, "time");
		//time+= .0016;
		//auto sinTime = sin(time);
		//const float value = time;
		//glUniform1f(timeLocation, sinTime);

		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		glfwPollEvents();		
	}

	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteProgram(shaderProgram);

	//destroy window and terminate glfw
	glfwDestroyWindow(window);	
	glfwTerminate();

	return 0;
}