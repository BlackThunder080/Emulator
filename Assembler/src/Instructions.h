#pragma once
#include <map>
#include <string>
#include <cstdint>

std::map<std::string, uint8_t> InstructionMap({
	{ "mw",  0x00 },
	{ "lw",  0x01 },
	{ "sw",  0x02 },
	{ "li",  0x03 },
	{ "lui", 0x04 },
	{ "b",   0x05 },
});