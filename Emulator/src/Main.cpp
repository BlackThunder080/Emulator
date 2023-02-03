#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "ImGuiGLStuff/imgui_impl_glfw.h"
#include "ImGuiGLStuff/imgui_impl_opengl3.h"

#include "Emulator/CPU.h"
#include "Emulator/GPU.h"


int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1600, 900, "Emulator", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to intialize GLAD :( \n";
		exit(-1);
	}

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int x, int y) {
		glViewport(0, 0, x, y);
	});

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 20);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 10.0f;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();

	CPU cpu;
	cpu.LoadRomFile("res/bios.bin");
		
	while (!glfwWindowShouldClose(window))
	{
		// Update
		cpu.ExecuteCycle();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("CPU State");
		ImGui::Text("PC: %04X", cpu.pc);
		for (int i = 0; i < cpu.registers.size(); i++)
			ImGui::Text("R%02d: 0x%08X  =  %d / %ff", i, cpu.registers[i], cpu.registers[i], *(float*)&cpu.registers[i]);
		ImGui::End();
		ImGui::Begin("VRAM");
		for (int i = 0; i < cpu.registers.size(); i++)
		{
			if (i % 2 != 0)
				ImGui::SameLine();
			ImGui::Text("%f", ((float*)cpu.ram)[i]);
		}
		ImGui::End();
		
		ImGui::EndFrame();

	
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		cpu.GetGPU()->RenderMeshes();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
}