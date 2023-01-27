#pragma once
#include <cstdint>
#include <memory>


template<typename T> T CreateInstructionFromBytes(const uint8_t bytes[4])
{
	T i;
	std::memcpy((void*)&i, bytes, 4);
	return i;
}

template<typename T> T CreateInstructionFromBytes(const uint32_t bytes)
{
	T i;
	std::memcpy((void*)&i, &bytes, 4);
	return i;
}


#pragma pack(push, 1)
struct InstructionR
{
	size_t opcode : 6;
};

struct InstructionI
{
	size_t opcode : 6;
	size_t rs : 5;
	size_t rd : 5;
	size_t imm : 16;
};

struct InstructionA
{
	size_t opcode : 6;
	size_t address : 24;
};
#pragma pack(pop)