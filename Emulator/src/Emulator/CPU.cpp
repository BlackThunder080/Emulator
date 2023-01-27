#include <iostream>
#include <fstream>
#include <bitset>

#include "CPU.h"
#include "Instruction.h"


static inline uint32_t bit_range(uint32_t val, uint8_t start, uint8_t end)
{
	return ((val >> start) & ((1 << (end - start)) - 1));
}


CPU::CPU()
{
	registers = std::array<uint32_t, 32>({}); pc = 0;
	addr = new uint8_t[16 * 1024 * 1024];
	rom = addr + 0x000000;
	ram = addr + 0x080000;
}

CPU::~CPU()
{
	delete addr;
}

void CPU::RunCycle()
{
	uint32_t instructionbytes = *(uint32_t*)(addr + pc); pc += 4;
	switch ((instructionbytes >> 4) & 0b11)
	{
	case 0b00: // R Type
	{
		InstructionR instruction = CreateInstructionFromBytes<InstructionR>(instructionbytes);
		break;
	}
	case 0b01: // I Type
	{
		InstructionI instruction = CreateInstructionFromBytes<InstructionI>(instructionbytes);
		switch (instruction.opcode)
		{
		case 0x10: // LI
			registers[instruction.rd] = instruction.imm;
			break;
		case 0x11: // LUI
			registers[instruction.rd] = ((uint32_t)(instruction.imm << 16) | (uint32_t)(registers[instruction.rd] & 0xffff));
			break;
		case 0x12: // LW
			registers[instruction.rd] = *(uint32_t*)(addr + instruction.imm + registers[instruction.rs]);
			break;
		case 0x13: // SW
			*(uint32_t*)(addr + instruction.imm + registers[instruction.rd]) = registers[instruction.rs];
			break;
		}
		break;
	}
	case 0b10: // A Type
	{
		InstructionA instruction = CreateInstructionFromBytes<InstructionA>(instructionbytes);
		switch (instruction.opcode)
		{
		case 0x20: // Branch
			pc = instruction.address;
			break;
		}
		break;
	}

	}
}

void CPU::LoadRomFile(std::string filename)
{
	std::ifstream romfile("res/bios.bin", std::ios::binary);
	romfile.read((char*)rom, 4 * 1024 * 1024);
	romfile.close();
}