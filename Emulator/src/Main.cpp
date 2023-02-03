#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	
	std::map<std::string, bool> popups = {
		{ "Load ROM", false },
	};

	while (!glfwWindowShouldClose(window))
	{
		// Update
		if (cpu.running)
			cpu.ExecuteCycle();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if (popups["Load ROM"])
		{
			ImGui::Begin("Load ROM", &popups["Load ROM"]);

			if (ImGui::Button(".."))
				std::filesystem::current_path(std::filesystem::current_path().parent_path());
			for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
			{
				if (entry.is_directory())
				{
					if (ImGui::Button(entry.path().filename().string().c_str()))
						std::filesystem::current_path(entry.path());
				}
				else
				{
					if (ImGui::Button(entry.path().filename().string().c_str()))
						cpu.LoadRomFile(entry.path().string());
				}
			}
			ImGui::End();
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Load ROM...", "Ctrl+O", &popups["Load ROM"]);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

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