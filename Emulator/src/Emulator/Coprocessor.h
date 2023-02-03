#pragma once
#include <cstdint>
#include "CPU.h"

class CPU;

class Coprocessor
{
public:
	virtual void ExecuteCycle() = 0;
	virtual void Call(CPU* cpu) = 0;
};