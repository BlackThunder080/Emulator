#include <iostream>
#include <fstream>
#include <bitset>

#include "Instruction.h"
#include "CPU.h"
#include "FPU.h"
#include "GPU.h"


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

	coprocessors[0] = std::make_unique<FPU>();
	coprocessors[1] = std::make_unique<GPU>();
}

CPU::~CPU()
{
	delete addr;
}

void CPU::ExecuteCycle()
{
	uint32_t instructionbytes = *(uint32_t*)(addr + pc); pc += 4;
	InstructionInfo instructioninfo = InstructionInfoFromOpcode[instructionbytes & 0b111111];

	switch (instructioninfo.type)
	{
	case InstructionType::Register:
	{
		InstructionR instruction(instructionbytes);
		switch (instruction.opcode)
		{
		case Opcode::CORD:
			//registers[instruction.r1] = coprocessors[instruction.r2]->Read(this, instruction.r3);
			break;
		case Opcode::COWR:
			coprocessors[instruction.r1]->Write(this, instruction.r2, registers[instruction.r3]);
			break;
		default:
			break;
		}
		break;
	}
	case InstructionType::Immediate:
	{
		InstructionI instruction(instructionbytes);
		switch (instruction.opcode)
		{
		case Opcode::LI:
			registers[instruction.rd] = instruction.imm;
			break;
		case Opcode::LUI:
			registers[instruction.rd] = ((uint32_t)(instruction.imm << 16) | (uint32_t)(registers[instruction.rd] & 0xffff));
			break;
		case Opcode::LW:
			registers[instruction.rd] = *(uint32_t*)(addr + instruction.imm + registers[instruction.rs]);
			break;
		case Opcode::SW:
			*(uint32_t*)(addr + instruction.imm + registers[instruction.rd]) = registers[instruction.rs];
			break;
		default:
			break;
		}
		break;
	}
	case InstructionType::Address:
	{
		InstructionA instruction(instructionbytes);
		switch (instruction.opcode)
		{
		case Opcode::B:
			pc = instruction.address;
			break;
		default:
			break;
		}
		break;
	}
	case InstructionType::None:
	{
		Instruction instruction(instructionbytes);
		switch (instruction.opcode)
		{
		default:
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