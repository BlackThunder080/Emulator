#pragma once
#include <cstdint>
#include "CPU.h"


class Coprocessor
{
public:
	virtual void ExecuteCycle() = 0;
	virtual uint32_t Read(class CPU* cpu, uint8_t address) = 0;
	virtual void Write(class CPU* cpu, uint8_t address, uint32_t value) = 0;
};