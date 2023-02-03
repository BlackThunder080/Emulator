#pragma once
#include <array>
#include <memory>
#include <string>

#include "Coprocessor.h"


class GPU;

class CPU
{
public:
	CPU();
	~CPU();

	void ExecuteCycle();
	void LoadRomFile(std::string filepath);

	std::array<uint32_t, 32> registers;
	uint32_t pc;
	uint8_t* addr;
	uint8_t* rom;
	uint8_t* ram;

	GPU* GetGPU() { return (GPU*)coprocessors[1].get(); }

	bool running = false;

private:
	std::array<std::unique_ptr<class Coprocessor>, 32> coprocessors;
};
