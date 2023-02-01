#pragma once
#include <bit>
#include <iostream>

#include "FPU.h"

uint32_t FPU::Read(CPU* cpu, uint8_t address)
{
	switch (address)
	{
	case 0x00:
		return std::bit_cast<uint32_t>(a);
	case 0x01:
		return std::bit_cast<uint32_t>(b);
	case 0x02:
		return std::bit_cast<uint32_t>(a + b);
	case 0x03:
		return std::bit_cast<uint32_t>(a - b);
	case 0x04:
		return std::bit_cast<uint32_t>(a * b);
	case 0x05:
		return std::bit_cast<uint32_t>(a / b);
	}
	
	std::cerr << "Invalid FPU address 0x" << std::hex << address << std::endl;
	exit(1);
}

void FPU::Write(CPU* cpu, uint8_t address, uint32_t value)
{
	(address == 0 ? a : b) = std::bit_cast<float>(value);
}