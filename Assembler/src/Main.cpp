#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Instructions.h"


struct Token
{
	std::string string;
	void* value;
	enum Type
	{
		None,
		Register,
		Immediate,
	} type;
};


int main()
{
	std::ifstream infile("res/bios.asm");
	std::ofstream outfile("../Emulator/res/bios.bin", std::ios::binary);

	int linenumber = 1;
	std::string line;
	while (std::getline(infile, line))
	{
		std::string instruction;
		std::vector<Token> operands;

		instruction = line.substr(0, line.find(' '));
		line.erase(0, instruction.length());
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) { return c != ' '; }));

		size_t offset;
		do
		{
			offset = line.find(',');
			operands.push_back({ line.substr(0, offset) });
			line.erase(0, operands.back().string.length() + 1);
			line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) { return c != ' '; }));
		} while (offset != std::string::npos);


		for (auto& operand : operands)
		{
			if (operand.string._Starts_with("$"))
			{
				operand.value = (void*)(size_t)stoi(operand.string.substr(1));
				operand.type = Token::Type::Register;
				continue;
			}
			else if (operand.string.find_first_not_of("0123456789") == std::string::npos)
			{
				operand.value = (void*)stoull(operand.string);
				operand.type = Token::Type::Immediate;
				continue;
			}
			else if (operand.string._Starts_with("0x"))
			{
				operand.value = (void*)stoull(operand.string, nullptr, 16);
				operand.type = Token::Type::Immediate;
				continue;
			}
			else
			{
				std::cerr << "Invalid operand on line " << linenumber << std::endl;
				exit(1);
			}
		}

		uint8_t bytes[4];

		uint8_t opcode = InstructionMap[instruction];
		switch ((opcode >> 4) & 0b11)
		{
		case 0b00: // R Type
		{
			break;
		}
		case 0b01: // I Type
		{
			uint8_t rs, rd;  uint16_t imm;
			if (operands.size() == 2)
			{
				if (operands[0].type == Token::Type::Register && operands[1].type == Token::Type::Immediate)
				{
					rs  = 0;
					rd  = (uint8_t)(size_t)operands[0].value;
					imm = (uint16_t)(size_t)operands[1].value;
				}
				else
				{
					std::cerr << "Invalid combination of operands on line " << linenumber << std::endl;
					exit(1);
				}
			}
			else if (operands.size() == 3)
			{
				if (operands[0].type == Token::Type::Register && operands[1].type == Token::Type::Register && operands[2].type == Token::Type::Immediate)
				{
					rs  =  (uint8_t)(size_t)operands[0].value;
					rd  =  (uint8_t)(size_t)operands[1].value;
					imm = (uint16_t)(size_t)operands[2].value;
				}
				else
				{
					std::cerr << "Invalid combination of operands on line " << linenumber << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cerr << "Wrong number of operands on line " << linenumber << std::endl;
				exit(1);
			}
			bytes[0] = (opcode) | ((rs & 0b11) << 6);
			bytes[1] = (rs >> 2) | (rd << 5);
			bytes[2] = (imm & 0xff);
			bytes[3] = ((imm >> 8) & 0xff);
			break;
		}
		case 0b10: // A Type
		{
			break;
		}
		}

		outfile.write((char*)bytes, 4);

		linenumber++;
	}

	return 0;
}