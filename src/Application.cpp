#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>

#include "tests/TestClearColor.h"
#include <tests\TestTexture2D.h>
#include "tests/TestPong.h"
#include <InputManager.h>


static void MatrixToString(glm::mat4 matrix)
{
	std::cout << matrix[0].x << " " << matrix[0].y << " " << matrix[0].z << " " << matrix[0].w << " " << std::endl;
	std::cout << matrix[1].x << " " << matrix[1].y << " " << matrix[1].z << " " << matrix[1].w << " " << std::endl;
	std::cout << matrix[2].x << " " << matrix[2].y << " " << matrix[2].z << " " << matrix[2].w << " " << std::endl;
	std::cout << matrix[3].x << " " << matrix[3].y << " " << matrix[3].z << " " << matrix[3].w << " " << std::endl;
}

void ProcessInput(GLFWwindow * window, InputManager * input)
{
	//W up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		input->ChangeKeyState("W", true);
	}
	else
	{
		input->ChangeKeyState("W", false);
	}

	//S down
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		input->ChangeKeyState("S", true);
	}
	else
	{
		input->ChangeKeyState("S", false);
	}

	//A left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		input->ChangeKeyState("A", true);
	}
	else
	{
		input->ChangeKeyState("A", false);
	}

	//D right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		input->ChangeKeyState("D", true);
	}
	else
	{
		input->ChangeKeyState("D", false);
	}

	//UP
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		input->ChangeKeyState("UP", true);
	}
	else
	{
		input->ChangeKeyState("UP", false);
	}

	//DOWN
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		input->ChangeKeyState("DOWN", true);
	}
	else
	{
	input->ChangeKeyState("DOWN", false);
	}

}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR GLEW!" << std::endl;
	}


	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Renderer renderer;

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	//testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
	testMenu->RegisterTest<test::TestPong>("Pong");

	InputManager inputManager;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		ProcessInput(window, &inputManager);
		
		/* Render here */
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		if (currentTest)
		{
			currentTest->OnUpdate(deltaTime, &inputManager);
			currentTest->OnRender();
			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("<--"))
			{
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete currentTest;
	if (currentTest != testMenu)
		delete testMenu;

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}