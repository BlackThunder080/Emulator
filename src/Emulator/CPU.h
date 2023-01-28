#pragma once
#include <array>

class CPU
{
public:
	CPU();
	~CPU();

	void RunCycle();
	void LoadRomFile(std::string filename);

	std::array<uint32_t, 32> registers;
	uint32_t pc;
	uint8_t* addr;
	uint8_t* rom;
	uint8_t* ram;
};
