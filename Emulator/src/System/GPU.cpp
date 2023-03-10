#include <glad/glad.h>
#include <memory>
#include "GPU.h"
#include <iostream>


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

GPU::GPU()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glCreateVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	auto vertshader = glCreateShader(GL_VERTEX_SHADER);
	auto fragshader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vertsource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		
		void main()
		{
		    gl_Position = vec4(aPos, 0.0, 1.0);
		}
	)";
	const char* fragsource = R"(
		#version 330 core
		out	vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		} 
	)";
	glShaderSource(vertshader, 1, &vertsource, nullptr);
	glShaderSource(fragshader, 1, &fragsource, nullptr);
	glCompileShader(vertshader);
	glCompileShader(fragshader);

	auto program = glCreateProgram();
	glAttachShader(program, vertshader);
	glAttachShader(program, fragshader);
	glLinkProgram(program);
	glUseProgram(program);
}

void GPU::ExecuteCycle()
{
}

void GPU::Call(CPU* cpu)
{
	switch (cpu->registers[1])
	{
	case 0x00:
		for (auto [buffer, vertices] : m_VBOs)
			glDeleteBuffers(1, &buffer);
		m_VBOs.clear();
		break;
	case 0x01:
		m_VBOs.push_back({ 0, 0 });
		glBindVertexArray(m_VertexArray);
		glGenBuffers(1, &m_VBOs.back().first);
		break;
	case 0x02:
		m_VBOs[cpu->registers[2]].second = cpu->registers[3];
		glBindVertexArray(m_VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[cpu->registers[2]].first);
		glBufferData(GL_ARRAY_BUFFER, m_VBOs[cpu->registers[2]].second * 2 * sizeof(float), cpu->addr + cpu->registers[4], GL_STATIC_DRAW);
		break;
	}
}

void GPU::RenderMeshes()
{
	glBindVertexArray(m_VertexArray);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	for (auto [buffer, vertices] : m_VBOs)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}
}
