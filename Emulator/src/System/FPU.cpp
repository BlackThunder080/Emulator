#include <bit>
#include <iostream>

#include "FPU.h"


void FPU::ExecuteCycle()
{
}

void FPU::Call(CPU* cpu)
{
	switch (cpu->registers[1])
	{
	case 0x00:
		cpu->registers[0] = std::bit_cast<uint32_t>((float)cpu->registers[2]); // From int to float
		break;
	case 0x01:
		cpu->registers[0] = (uint32_t)std::bit_cast<float>(cpu->registers[2]); // From float to int
		break;
	case 0x02:
		cpu->registers[0] = std::bit_cast<uint32_t>(std::bit_cast<float>(cpu->registers[2]) + std::bit_cast<float>(cpu->registers[3]));
		break;
	case 0x03:
		cpu->registers[0] = std::bit_cast<uint32_t>(std::bit_cast<float>(cpu->registers[2]) - std::bit_cast<float>(cpu->registers[3]));
		break;
	case 0x04:
		cpu->registers[0] = std::bit_cast<uint32_t>(std::bit_cast<float>(cpu->registers[2]) * std::bit_cast<float>(cpu->registers[3]));
		break;
	case 0x05:
		cpu->registers[0] = std::bit_cast<uint32_t>(std::bit_cast<float>(cpu->registers[2]) / std::bit_cast<float>(cpu->registers[3]));
		break;
	default:
		std::cerr << "Invalid FPU syscall 0x" << std::hex << cpu->registers[1] << std::endl;
		exit(1);
	}
}