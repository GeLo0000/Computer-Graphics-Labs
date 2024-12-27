#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <numbers>
 
#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


#define M_PI 3.14159265358979323846

const unsigned int width = 1600;
const unsigned int height = 900;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

std::vector<GLfloat> generatePlaneVertices(int gridSize, float stepSize)
{
	std::vector<GLfloat> vertices;

	// ��������� �� ������� ��������� x � y � ������
	for (int i = 0; i <= gridSize; ++i)
	{
		for (int j = 0; j <= gridSize; ++j)
		{
			// ������ ���������� x � y
			float x = i * stepSize - (gridSize * stepSize / 2.0f);  // �������� �������
			float y = j * stepSize - (gridSize * stepSize / 2.0f);

			// ���������� Z �� ��������
			float z = sqrt(fabs(y)) + sqrt(fabs(x));

			// ������ ���������� ��� �������
			vertices.push_back(x);  // x
			vertices.push_back(y);  // y
			vertices.push_back(z);  // z

			// ������� ��� ������� (����� ������ ������ ��� ���������)
			vertices.push_back(0.83f);  // r
			vertices.push_back(0.70f);  // g
			vertices.push_back(0.44f);  // b

			// �������� ����������
			vertices.push_back((float)i / gridSize);  // texCoord x
			vertices.push_back((float)j / gridSize);  // texCoord y
		}
	}

	return vertices;
}

// ������ ������� ��� ��������� �������
std::vector<GLuint> generatePlaneIndices(int gridSize)
{
	std::vector<GLuint> indices;

	// ��������� ���������� ��� ������� �������� � ����
	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			GLuint topLeft = i * (gridSize + 1) + j;
			GLuint topRight = topLeft + 1;
			GLuint bottomLeft = (i + 1) * (gridSize + 1) + j;
			GLuint bottomRight = bottomLeft + 1;

			// ������ ��� ���������� ��� ������� ��������
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	return indices;
}


std::vector<GLfloat> generateCubeVertices(float size = 1.0f) {
	std::vector<GLfloat> vertices = {
		// �������            // �������          // �������� ����������
		// ������� �����
		-size, -size,  size,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		 size, -size,  size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 size,  size,  size,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-size,  size,  size,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

		// ����� �����
		-size, -size, -size,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 size, -size, -size,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 size,  size, -size,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-size,  size, -size,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f,

		// ˳�� �����
		-size, -size, -size,  0.2f, 0.2f, 0.2f,  0.0f, 0.0f,
		-size,  size, -size,  0.3f, 0.3f, 0.3f,  1.0f, 0.0f,
		-size,  size,  size,  0.4f, 0.4f, 0.4f,  1.0f, 1.0f,
		-size, -size,  size,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f,

		// ����� �����
		 size, -size, -size,  0.6f, 0.6f, 0.6f,  0.0f, 0.0f,
		 size,  size, -size,  0.7f, 0.7f, 0.7f,  1.0f, 0.0f,
		 size,  size,  size,  0.8f, 0.8f, 0.8f,  1.0f, 1.0f,
		 size, -size,  size,  0.9f, 0.9f, 0.9f,  0.0f, 1.0f,

		 // ����� �����
		 -size, -size, -size,  1.0f, 0.5f, 0.5f,  0.0f, 0.0f,
		  size, -size, -size,  0.5f, 1.0f, 0.5f,  1.0f, 0.0f,
		  size, -size,  size,  0.5f, 0.5f, 1.0f,  1.0f, 1.0f,
		 -size, -size,  size,  0.3f, 0.3f, 0.3f,  0.0f, 1.0f,

		 // ������ �����
		 -size,  size, -size,  0.4f, 0.4f, 0.4f,  0.0f, 0.0f,
		  size,  size, -size,  0.6f, 0.6f, 0.6f,  1.0f, 0.0f,
		  size,  size,  size,  0.8f, 0.8f, 0.8f,  1.0f, 1.0f,
		 -size,  size,  size,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
	};

	return vertices;
}

std::vector<GLuint> generateCubeIndices() {
	std::vector<GLuint> indices = {
		// ������� �����
		0, 1, 2, 0, 2, 3,
		// ����� �����
		4, 5, 6, 4, 6, 7,
		// ˳�� �����
		8, 9, 10, 8, 10, 11,
		// ����� �����
		12, 13, 14, 12, 14, 15,
		// ����� �����
		16, 17, 18, 16, 18, 19,
		// ������ �����
		20, 21, 22, 20, 22, 23
	};

	return indices;
}

std::vector<GLfloat> generateTorusVertices(float R = 1.0f, float r = 0.4f, int numMajor = 32, int numMinor = 16) {
	std::vector<GLfloat> vertices;
	for (int i = 0; i < numMajor; ++i) {
		float majorAngle = i * 2.0f * M_PI / numMajor;
		float cosMajor = cos(majorAngle);
		float sinMajor = sin(majorAngle);
		for (int j = 0; j < numMinor; ++j) {
			float minorAngle = j * 2.0f * M_PI / numMinor;
			float cosMinor = cos(minorAngle);
			float sinMinor = sin(minorAngle);

			float x = (R + r * cosMinor) * cosMajor;
			float y = (R + r * cosMinor) * sinMajor;
			float z = r * sinMinor;
			float u = (float)i / (float)numMajor;
			float v = (float)j / (float)numMinor;

			vertices.insert(vertices.end(), { x, y, z, u, v, 1.0f, 0.0f, 0.0f });
		}
	}
	return vertices;
}

std::vector<GLuint> generateTorusIndices(int numMajor = 32, int numMinor = 16) {
	std::vector<GLuint> indices;
	for (unsigned int i = 0; i < numMajor; ++i) {
		for (unsigned int j = 0; j < numMinor; ++j) {
			unsigned int nextI = (i + 1) % numMajor;
			unsigned int nextJ = (j + 1) % numMinor;

			indices.insert(indices.end(), {
				i * numMinor + j,
				nextI * numMinor + j,
				nextI * numMinor + nextJ,
				i * numMinor + j,
				nextI * numMinor + nextJ,
				i * numMinor + nextJ
				});
		}
	}
	return indices;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Lab_2", NULL, NULL);
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



	Shader shaderProgram("default.vert", "default.frag");



	// Texture for plane
	Texture planeTex("Images/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	planeTex.texUnit(shaderProgram, "tex0", 0);

	int gridSize = 15;  // ����� ����
	float stepSize = 0.5f;  // ���� �� �������

	// �������� ������� �� ������� ��� �������
	std::vector<GLfloat> planeVertices = generatePlaneVertices(gridSize, stepSize);
	std::vector<GLuint> planeIndices = generatePlaneIndices(gridSize);

	// ��������� VAO, VBO, EBO ��� �������
	VAO planeVAO;
	planeVAO.Bind();

	VBO planeVBO(planeVertices.data(), planeVertices.size() * sizeof(GLfloat));
	EBO planeEBO(planeIndices.data(), planeIndices.size() * sizeof(GLuint));

	// ����'����� ��������
	planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);  // �������
	planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // ����
	planeVAO.LinkAttrib(planeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // �������� ����������

	planeVAO.Unbind();
	planeVBO.Unbind();
	planeEBO.Unbind();



	// Texture for cube
	Texture cubeTex("Images/logo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	cubeTex.texUnit(shaderProgram, "tex0", 0);

	// �������� ������� �� ������� ��� ����
	std::vector<GLfloat> cubeVertices = generateCubeVertices();
	std::vector<GLuint> cubeIndices = generateCubeIndices();

	// ��������� VAO, VBO, EBO ��� ����
	VAO cubeVAO;
	cubeVAO.Bind();

	VBO cubeVBO(cubeVertices.data(), cubeVertices.size() * sizeof(GLfloat));
	EBO cubeEBO(cubeIndices.data(), cubeIndices.size() * sizeof(GLuint));

	// ����'����� ��������
	cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);  // �������
	cubeVAO.LinkAttrib(cubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // ����
	cubeVAO.LinkAttrib(cubeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // �������� ����������

	cubeVAO.Unbind();
	cubeVBO.Unbind();
	cubeEBO.Unbind();



	// Texture for torus
	Texture torusTex("Images/red.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	torusTex.texUnit(shaderProgram, "tex0", 0);

	std::vector<GLfloat> torusVertices = generateTorusVertices();
	std::vector<GLuint> torusIndices = generateTorusIndices();

	// ��������� VAO, VBO, EBO ��� ����
	VAO torusVAO;
	torusVAO.Bind();

	VBO torusVBO(torusVertices.data(), torusVertices.size() * sizeof(GLfloat));
	EBO torusEBO(torusIndices.data(), torusIndices.size() * sizeof(GLuint));

	// ����'����� ��������
	torusVAO.LinkAttrib(torusVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);  // �������
	torusVAO.LinkAttrib(torusVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // ����
	torusVAO.LinkAttrib(torusVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // �������� ����������

	torusVAO.Unbind();
	torusVBO.Unbind();
	torusEBO.Unbind();


	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// Inputs
		processInput(window);

		// Rendering commands
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		// Camera
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");

		// ������� (���������� � ����� �����)
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::rotate(planeModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		planeModel = glm::translate(planeModel, glm::vec3(0.0f, 0.0f, -5.0f)); // ����
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(planeModel));
		planeTex.Bind();
		planeVAO.Bind();
		glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);
		planeVAO.Unbind();

		// ��� (������� ������ �� �������)
		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, glm::vec3(-1.5f, 0.0f, 0.0f)); // ˳�����
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));
		cubeTex.Bind();
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
		cubeVAO.Unbind();
		
		// ��� (������� �������� �� �������)
		glm::mat4 torusModel = glm::mat4(1.0f);
		torusModel = glm::translate(torusModel, glm::vec3(1.5f, 0.0f, 0.0f)); // ��������
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torusModel));
		torusTex.Bind();
		torusVAO.Bind();
		glDrawElements(GL_TRIANGLES, torusIndices.size(), GL_UNSIGNED_INT, 0);
		torusVAO.Unbind();
		
		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	planeTex.Delete();
	cubeTex.Delete();
	torusTex.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
