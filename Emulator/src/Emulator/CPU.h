#pragma once
#include <array>
#include <memory>

#include "Coprocessor.h"


class CPU
{
public:
	CPU();
	~CPU();

	void ExecuteCycle();
	void LoadRomFile(std::string filename);

	std::array<uint32_t, 32> registers;
	uint32_t pc;
	uint8_t* addr;
	uint8_t* rom;
	uint8_t* ram;
private:
	std::array<std::unique_ptr<Coprocessor>, 32> coprocessors;
};
