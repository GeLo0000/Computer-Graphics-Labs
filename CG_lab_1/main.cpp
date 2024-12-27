#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(0.2f, 0.5f, 0.0f, 1.0f);\n"
"}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float* vertices, int vertexCount, unsigned int VBO)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
	{
		int i = 1;
		while (i < vertexCount)
		{
			vertices[i] = vertices[i] + 0.01f;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float), vertices);
	}
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
	{
		int i = 1;
		while (i < vertexCount)
		{
			vertices[i] = vertices[i] - 0.01f;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float), vertices);
	}
	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
	{
		int i = 0;
		while (i < vertexCount)
		{
			vertices[i] = vertices[i] + 0.01f;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float), vertices);
	}
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
	{
		int i = 0;
		while (i < vertexCount)
		{
			vertices[i] = vertices[i] - 0.01f;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float), vertices);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Lab_1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	float vertices[] = {
		-0.9f, (0.4 * sqrt(2) + 0.2), 0.0f,
		(-0.9 + 0.2 * sqrt(2)), (0.2 + (0.4 * sqrt(2)) / 2), 0.0f,
		-0.5f, 0.6f, 0.0f,
		-0.9f, 0.2f, 0.0f,
		-0.5f, 0.2f, 0.0f,

		-0.9f, -0.2f, 0.0f,
		-0.5f, -0.2f, 0.0f,

		-0.5f, 0.0f, 0.0f,
		0.1f, 0.0f, 0.0f,
		-0.5f, -0.6f, 0.0f,

		-0.2f, -0.3f, 0.0f,
		0.4f, -0.3f, 0.0f,

		0.7f, 0.0f, 0.0f,
		0.7f, -0.6f, 0.0f,

		0.75f, 0.3f, 0.0f,
		0.95f, 0.5f, 0.0f,
		0.9f, 0.2f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		2, 3, 4,

		3, 4, 6,
		3, 5, 6,

		7, 8, 9,
		8, 10, 11,
		8, 12, 13,

		12, 14, 16,
		14, 15, 16
	};

	int vertexCount = sizeof(vertices) / sizeof(float);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window, vertices, vertexCount, VBO);

		// rendering commands here
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
