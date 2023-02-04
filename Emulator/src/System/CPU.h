#pragma once
#include <array>
#include <memory>
#include <string>

#include "Coprocessor.h"


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

	class FPU* GetFPU() { return (class FPU*) coprocessors[0].get(); }
	class GPU* GetGPU() { return (class GPU*) coprocessors[1].get(); }
	class Controller* GetController() { return (class Controller*) coprocessors[2].get(); }

	bool running = false;

private:
	std::array<std::unique_ptr<class Coprocessor>, 32> coprocessors;
};
