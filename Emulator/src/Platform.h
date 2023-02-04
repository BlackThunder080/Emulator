#pragma once
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Platform
{
	inline void GlfwInit()
	{
		glfwInit();
	}
	inline void ImGuiInit()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
	}

	inline void GlfwClose()
	{
		glfwTerminate();
	}
	inline void ImGuiClose()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	GLFWwindow* GlfwCreateWindow(const char* name, int width, int height)
	{
		GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int x, int y) {
			glViewport(0, 0, x, y);
		});

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to intialize GLAD :( \n";
			exit(-1);
		}

		return window;
	}
	void ImGuiInitForGLFW(GLFWwindow* window)
	{
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 20);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 10.0f;
	}

	inline void Init()
	{
		GlfwInit();
		ImGuiInit();
	}
	inline void Close()
	{
		GlfwClose();
		ImGuiClose();
	}
}