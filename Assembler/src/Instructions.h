#pragma once
#include <map>
#include <string>
#include <cstdint>

std::map<std::string, uint8_t> InstructionMap({
	{ "li",  0x10 },
	{ "lui", 0x11 },
	{ "lw",  0x12 },
	{ "sw",  0x13 },
	{ "b",   0x20 },
});