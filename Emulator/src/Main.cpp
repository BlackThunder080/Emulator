#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "ImGuiGLStuff/imgui_impl_glfw.h"
#include "ImGuiGLStuff/imgui_impl_opengl3.h"

#include "Emulator/CPU.h"


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

	unsigned int vertexarray, vertexbuffer;

	glGenVertexArrays(1, &vertexarray);
	glBindVertexArray(vertexarray);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);
		
	while (!glfwWindowShouldClose(window))
	{
		// Update
		cpu.RunCycle();
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), cpu.ram, GL_DYNAMIC_DRAW);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::Begin("CPU State");
		ImGui::Text("PC: %04X", cpu.pc);
		ImGui::Text("%f, %f", ((float*)cpu.ram)[0], ((float*)cpu.ram)[1]);
		ImGui::Text("%f, %f", ((float*)cpu.ram)[2], ((float*)cpu.ram)[3]);
		ImGui::Text("%f, %f", ((float*)cpu.ram)[4], ((float*)cpu.ram)[5]);
		for (int i = 0; i < cpu.registers.size(); i++)
			ImGui::Text("R%02d: 0x%08X  =  %d / %ff", i, cpu.registers[i], cpu.registers[i], *(float*)&cpu.registers[i]);
		ImGui::End();
		
		ImGui::EndFrame();

		// Render
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glBindVertexArray(vertexarray);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
}