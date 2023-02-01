#pragma once
#include <map>
#include <vector>
#include <string>


// ============================================= //
struct Instruction
{
	size_t opcode : 6;

	Instruction() : opcode(0) {}
	Instruction(uint32_t bytes)
		: opcode(bytes & 0b111111) {};
};

struct InstructionR : public Instruction
{
	size_t opcode : 6;
	size_t r1     : 5;
	size_t r2     : 5;
	size_t r3     : 5;

	InstructionR(uint32_t bytes)
		: opcode((bytes >> 0) & 0b111111), r1((bytes >> 6) & 0b11111), r2((bytes >> 11) & 0b11111), r3((bytes >> 16) & 0b11111) {}
};

struct InstructionI : public Instruction
{
	size_t opcode : 6;
	size_t rs : 5;
	size_t rd : 5;
	size_t imm : 16;

	InstructionI(uint32_t bytes)
		: opcode((bytes >> 0) & 0b111111), rs((bytes >> 6) & 0b11111), rd((bytes >> 11) & 0b11111), imm(bytes >> 16) {}
};

struct InstructionA : public Instruction
{
	size_t opcode : 6;
	size_t address : 24;

	InstructionA(uint32_t bytes)
		: opcode((bytes >> 0) & 0b111111), address((bytes >> 6)) {}
};

// ============================================= //

enum Opcode : uint8_t
{
	MW,
	LW,
	SW,
	LI,
	LUI,
	B,
	CORD,
	COWR,
};

enum class InstructionType
{
	Register,
	Immediate,
	Address,
	None,
};

enum class OperandType
{
	Register,
	Immediate,
};

struct InstructionInfo
{
	std::string name;
	uint8_t opcode = 0;
	InstructionType type = InstructionType::None;
	std::vector<OperandType> operands;
};

// ============================================= //

std::map<uint8_t, InstructionInfo> InstructionInfoFromOpcode = {
	{ Opcode::MW,   { "mw",     Opcode::MW,   InstructionType::Register  } },
	{ Opcode::LW,   { "lw",     Opcode::LW,   InstructionType::Immediate } },
	{ Opcode::SW,   { "sw",     Opcode::SW,   InstructionType::Immediate } },
	{ Opcode::LI,   { "li",     Opcode::LI,   InstructionType::Immediate } },
	{ Opcode::LUI,  { "lui",    Opcode::LUI,  InstructionType::Immediate } },
	{ Opcode::B,    { "b",      Opcode::B,    InstructionType::Address   } },
	{ Opcode::CORD, { "cord",   Opcode::CORD, InstructionType::Register  } },
	{ Opcode::COWR, { "cowr",   Opcode::COWR, InstructionType::Register  } },
};

std::map<std::string, InstructionInfo> InstructionInfoFromName = {
	{ "mw",     InstructionInfoFromOpcode[ Opcode::MW   ] },
	{ "lw",     InstructionInfoFromOpcode[ Opcode::LW   ] },
	{ "sw",     InstructionInfoFromOpcode[ Opcode::SW   ] },
	{ "li",     InstructionInfoFromOpcode[ Opcode::LI   ] },
	{ "lui",    InstructionInfoFromOpcode[ Opcode::LUI  ] },
	{ "b",      InstructionInfoFromOpcode[ Opcode::B    ] },
	{ "cord",   InstructionInfoFromOpcode[ Opcode::CORD ] },
	{ "cowr",   InstructionInfoFromOpcode[ Opcode::COWR ] },
};