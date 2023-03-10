#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../../Emulator/src/System/Instruction.h"


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

inline const char* PopArgument(int& argc, char**& argv)
{
	const char* argument = argv[0];
	argc--;
	argv++;
	return argument;
}

inline void exit_with_message(int code, const char* message)
{
	std::cerr << message << std::endl;
	exit(code);
}



int main(int argc, char *argv[])
{
	std::map<std::string, std::string> cmd_args = {
		{ "in",  "a.in" },
		{ "out", "a.out" },
	};
	PopArgument(argc, argv);
	cmd_args["in"] = PopArgument(argc, argv);
	while (argc > 0)
	{
		if (std::strcmp(PopArgument(argc, argv), "-o") == 0)
			cmd_args["out"] = PopArgument(argc, argv);
		else
			exit_with_message(1, "Invalid Argument");
	}


	std::ifstream infile(cmd_args["in"]);
	std::ofstream outfile(cmd_args["out"], std::ios::binary);

	std::string line;

	std::map<std::string, size_t> labels;
	size_t address = 0;
	while (std::getline(infile, line))
	{	
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1, line.length());

		if (line == "")
			continue;
		if (labels.find(line.substr(0, line.find(':'))) != labels.end())
			continue;

		int offset = (int)line.find(':');
		if (offset != std::string::npos)
		{
			labels[line.substr(0, offset)] = address;
			
			// If we're at the end of the line, no need to increment address
			if (offset == line.length())
				continue;
		}
		address += 4;
	}

	infile.clear();
	infile.seekg(0);

	int linenumber = 0;
	while (std::getline(infile, line))
	{
		linenumber++;

		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1, line.length());

		if (line == "")
			continue;
		if (labels.find(line.substr(0, line.find(':'))) != labels.end())
			continue;

		std::string instruction;
		std::vector<Token> operands;

		instruction = line.substr(0, line.find(' '));
		line.erase(0, instruction.length());
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) { return c != ' ' && c != '\t'; }));

		size_t offset;
		do
		{
			offset = line.find(',');
			operands.push_back({ line.substr(0, offset) });
			line.erase(0, operands.back().string.length() + 1);
			line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](char c) { return c != ' ' && c != '\t'; }));
		} while (offset != std::string::npos);


		for (auto& operand : operands)
		{
			if (operand.string.starts_with("$"))
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
			else if (operand.string.starts_with("0x"))
			{
				operand.value = (void*)stoull(operand.string, nullptr, 16);
				operand.type = Token::Type::Immediate;
				continue;
			}
			else if (labels.find(operand.string) != labels.end())
			{
				operand.value = (void*)labels[operand.string];
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

		InstructionInfo instructioninfo = InstructionInfoFromName[instruction];
		switch (instructioninfo.type)
		{
		case InstructionType::Register:
		{
			uint8_t r1, r2, r3;
			if (operands.size() == 2)
			{
				if (operands[0].type == Token::Type::Register && operands[1].type == Token::Type::Register)
				{
					r1 = (uint8_t)(size_t)operands[0].value;
					r2 = (uint8_t)(size_t)operands[1].value;
					r3 = 0;
				}
				else
				{
					std::cerr << "Invalid combination of operands on line " << linenumber << std::endl;
					exit(1);
				}
			}
			else if (operands.size() == 3)
			{
				if (operands[0].type == Token::Type::Register && operands[1].type == Token::Type::Register && operands[2].type == Token::Type::Register)
				{
					r1 = (uint8_t)(size_t)operands[0].value;
					r2 = (uint8_t)(size_t)operands[1].value;
					r3 = (uint8_t)(size_t)operands[2].value;
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
			bytes[0] = (instructioninfo.opcode) | ((r1 & 0b11) << 6);
			bytes[1] = (r1 >> 2) | (r2 << 3);
			bytes[2] = (r3);
			break;
			break;
		}
		case InstructionType::Immediate:
		{
			uint8_t rs, rd;  uint16_t imm;
			if (operands.size() == 1)
			{
				if (operands[0].type == Token::Type::Immediate)
				{
					rs = 0;
					rd = 0;
					imm = (uint16_t)(size_t)operands[0].value;
				}
				else
				{
					std::cerr << "Invalid combination of operands on line " << linenumber << std::endl;
					exit(1);
				}
			}
			else if (operands.size() == 2)
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
			bytes[0] = (instructioninfo.opcode) | ((rs & 0b11) << 6);
			bytes[1] = (rs >> 2) | (rd << 3);
			bytes[2] = (imm & 0xff);
			bytes[3] = ((imm >> 8) & 0xff);
			break;
		}
		case InstructionType::Address: // A Type
		{
			uint32_t address;
			if (operands[0].type == Token::Type::Immediate)
			{
				address = (uint16_t)(size_t)operands[0].value;
			}
			else
			{
				std::cerr << "Invalid combination of operands on line " << linenumber << std::endl;
				exit(1);
			}
			bytes[0] = instructioninfo.opcode | ((address & 0b11) << 6);
			bytes[1] = ((address >>  2) & 0xff);
			bytes[2] = ((address >> 10) & 0xff);
			bytes[3] = ((address >> 18) & 0xff);
			break;
		}
		}

		outfile.write((char*)bytes, 4);
	}

	return 0;
}